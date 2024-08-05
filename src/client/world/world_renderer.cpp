#include "world_renderer.h"
#include <SDL2/SDL_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include "client/scenes/world_scene.h"
#include "client/client.h"
#include "core/file_loader.h"
#include "core/color.h"

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
    glm::mat4 viewTransformInverse = glm::inverse(viewTransform);

    screenBox.start = glm::vec2(viewTransformInverse * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f));
    screenBox.size = glm::vec2(viewTransformInverse * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f)) - screenBox.start; 
}

void WorldRenderer::updateSize(glm::ivec2 size, const WorldScene &scene) {
    shadowBuffer.updateSize(size);

    updateTransforms(scene);
}

void WorldRenderer::render(WorldScene &scene) {
    updateTransforms(scene);

    entities.render(scene);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getGLTexture());

    glActiveTexture(GL_TEXTURE1);
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
    backSpriteProgram.setTransform(viewTransform);
    shadowSpriteProgram.setTransform(shadowViewTransform);
    lightSpriteProgram.setTransform(viewTransform);

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

            client->spriteRenderer.renderMesh(section.frontMesh, shadowSpriteProgram);
        }
    }

    client->spriteRenderer.renderMesh(entities.spriteSystem.mesh, shadowSpriteProgram);

    // Render main scene
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowSize.x, windowSize.y);

    engine->renderer.clearScreen(backgroundColor);

    for (const BlockMesh *blockMesh : blockMeshes.chunks) {
        if (blockMesh == nullptr) {
            continue;
        }

        for (int sectionIndex = sectionStart; sectionIndex <= sectionEnd; sectionIndex++) {
            const BlockMeshSection &section = blockMesh->sections[sectionIndex];

            client->spriteRenderer.renderMesh(section.backMesh, backSpriteProgram);
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

            client->spriteRenderer.renderMesh(section.lightMesh, lightSpriteProgram);
        }
    }

    glDisable(GL_BLEND);
}

WorldRenderer::WorldRenderer(WorldScene &scene) :
    backSpriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment_back.glsl"),
    shadowSpriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment_shadow.glsl"),
    lightSpriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment_light.glsl") {

    // Set shadow offset
    shadowTransform = glm::translate(glm::mat4(1.0f), glm::vec3(glm::vec2(SHADOW_OFFSET), 0.0f));

    // Bind textures for back program
    GLint
        backTextureLocation = glGetUniformLocation(backSpriteProgram.getGLProgram(), "fTexture"),
        backShadowTextureLocation = glGetUniformLocation(backSpriteProgram.getGLProgram(), "fShadowTexture");

    glUseProgram(backSpriteProgram.getGLProgram());
    glUniform1i(backTextureLocation, 0);
    glUniform1i(backShadowTextureLocation, 1);

    loadTextureAtlas();

    backgroundColor = Color::parseHex("56f9d3");
}