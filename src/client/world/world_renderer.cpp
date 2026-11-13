#include "world_renderer.h"
#include "client/scenes/world_scene.h"
#include <SDL2/SDL_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include "core/file_loader.h"
#include "core/color.h"
#include "gfx/core/renderer.h"
#include "client/client.h"
#include "gfx/core/shader.h"

using namespace bf;

void WorldRenderer::loadTextureAtlas() {
    std::vector<std::string> texturePaths;
	FileLoader::getFilePaths("assets/textures/world", texturePaths);

	SDL_Surface *surface = textureAtlas.loadSurface(texturePaths);
	texture.loadSurface(surface);

    IMG_SavePNG(surface, "atlas.png");

	SDL_FreeSurface(surface);
}

void WorldRenderer::updateTransforms(const WorldScene &scene) {
    viewTransform = engine->getWindowTransform() * scene.camera.getTransform();
    shadowViewTransform = viewTransform * shadowTransform;

    // Update screen box
    glm::mat4 viewInverseTransform = glm::inverse(viewTransform);

    screenBox.start = glm::vec2(viewInverseTransform * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f));
    screenBox.size = glm::vec2(viewInverseTransform * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f)) - screenBox.start; 
}

void WorldRenderer::updateSize(glm::ivec2 size, WorldScene &scene) {
    shadowBuffer.updateSize(size);
    updateTransforms(scene);

    hud.updateMesh(scene);
}

void WorldRenderer::render(WorldScene &scene) {
    updateTransforms(scene);

    entities.render(scene);

    // Bind textures
    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture.getGLTexture());

    glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowBuffer.texture.getGLTexture());

    // Get visible chunks, including shadows
    Box2 screenBox = getScreenBox();

    glm::ivec2 blockStart = glm::floor(screenBox.start - SHADOW_OFFSET);
    glm::ivec2 blockEnd = glm::floor(screenBox.getEnd());

    BlockSample<BlockMesh> blockMeshes;
    blockMeshes.sampleBlocks(map.map, blockStart.x, blockEnd.x);

    // Get visible sections
    int sectionStart = glm::max(0, BlockMesh::getSectionIndex(blockStart.y));
    int sectionEnd = glm::min(BlockMesh::SECTION_COUNT - 1, BlockMesh::getSectionIndex(blockEnd.y));

    // Set transforms
    client->spriteProgram.setTransform(viewTransform);
    blockBackProgram.setTransform(viewTransform);
    blockShadowProgram.setTransform(shadowViewTransform);
    blockLightProgram.setTransform(viewTransform);

    // Render shadow to framebuffer
    glm::ivec2 windowSize = engine->getWindowSize();
    
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer.getGLFramebuffer());
    glViewport(0, 0, windowSize.x, windowSize.y);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const BlockMesh *blockMesh : blockMeshes.chunks) {
        if (blockMesh == nullptr) {
            continue;
        }

        for (int sectionIndex = sectionStart; sectionIndex <= sectionEnd; sectionIndex++) {
            const BlockMeshSection &section = blockMesh->sections[sectionIndex];

            client->spriteRenderer.renderMesh(section.frontMesh, blockShadowProgram);
        }
    }

    client->spriteRenderer.renderMesh(entities.spriteSystem.mesh, blockShadowProgram);

    // Render main scene
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowSize.x, windowSize.y);

    Renderer::clearScreen(backgroundColor);

    for (const BlockMesh *blockMesh : blockMeshes.chunks) {
        if (blockMesh == nullptr) {
            continue;
        }

        for (int sectionIndex = sectionStart; sectionIndex <= sectionEnd; sectionIndex++) {
            const BlockMeshSection &section = blockMesh->sections[sectionIndex];

            client->spriteRenderer.renderMesh(section.backMesh, blockBackProgram);
            client->spriteRenderer.renderMesh(section.frontMesh, client->spriteProgram);
        }
    }

    client->spriteRenderer.renderMesh(entities.spriteSystem.mesh, client->spriteProgram);

    // Render light
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);

    for (const BlockMesh *blockMesh : blockMeshes.chunks) {
        if (blockMesh == nullptr) {
            continue;
        }

        for (int sectionIndex = sectionStart; sectionIndex <= sectionEnd; sectionIndex++) {
            const BlockMeshSection &section = blockMesh->sections[sectionIndex];

            client->spriteRenderer.renderMesh(section.lightMesh, blockLightProgram);
        }
    }

    glDisable(GL_BLEND);

    hud.render(scene);
}

WorldRenderer::WorldRenderer() {
    // Create shaders
    GLuint
        glVertexShader = glCreateShader(GL_VERTEX_SHADER),
        glCommonShader = glCreateShader(GL_FRAGMENT_SHADER),
        glBlockBackShader = glCreateShader(GL_FRAGMENT_SHADER),
        glBlockShadowShader = glCreateShader(GL_FRAGMENT_SHADER),
        glBlockLightShader = glCreateShader(GL_FRAGMENT_SHADER);

	Shader::compileShader(glVertexShader, "assets/shaders/vertex.glsl");
    Shader::compileShader(glCommonShader, "assets/shaders/common.glsl");
    Shader::compileShader(glBlockBackShader, "assets/shaders/block_back.glsl");
    Shader::compileShader(glBlockShadowShader, "assets/shaders/block_shadow.glsl");
    Shader::compileShader(glBlockLightShader, "assets/shaders/block_light.glsl");

    blockBackProgram.attachShader(glVertexShader);
    blockBackProgram.attachShader(glCommonShader);
    blockBackProgram.attachShader(glBlockBackShader);
    blockBackProgram.link();

    blockShadowProgram.attachShader(glVertexShader);
    blockShadowProgram.attachShader(glCommonShader);
    blockShadowProgram.attachShader(glBlockShadowShader);
    blockShadowProgram.link();

    blockLightProgram.attachShader(glVertexShader);
    blockLightProgram.attachShader(glCommonShader);
    blockLightProgram.attachShader(glBlockLightShader);
    blockLightProgram.link();

	glDeleteShader(glVertexShader);
	glDeleteShader(glCommonShader);
    glDeleteShader(glBlockBackShader);
    glDeleteShader(glBlockShadowShader);
    glDeleteShader(glBlockLightShader);
    
    blockBackProgram.assignTexture(1, "fTexture");
    blockBackProgram.assignTexture(2, "fShadowTexture");

    blockShadowProgram.assignTexture(1, "fTexture");

    loadTextureAtlas();

    backgroundColor = Color::parseHex("56f9d3");

    // Set shadow offset
    shadowTransform = glm::translate(glm::mat4(1.0f), glm::vec3(glm::vec2(SHADOW_OFFSET), 0.0f));
}