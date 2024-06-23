#include "world_renderer.h"
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

void WorldRenderer::render(const WorldScene &scene) {
    updateTransforms(scene);

    entities.render(scene);

    // Get visible chunks, including shadows
    Box2 screenBox = scene.worldRenderer.getScreenBox();

    int blockStartX = (int)glm::floor(screenBox.start.x - SHADOW_OFFSET);
    int blockEndX = (int)glm::floor(screenBox.start.x + screenBox.size.x);

    BlockSample<BlockMesh> blockMeshes(scene.worldRenderer.map.map, blockStartX, blockEndX);

    // Render shadow to framebuffer
    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowBuffer.texture.getGLTexture());

    client->spriteProgram.setTransform(viewTransform);
    backSpriteProgram.setTransform(viewTransform);
    shadowSpriteProgram.setTransform(shadowViewTransform);
    
    glm::ivec2 windowSize = engine->getWindowSize();
    
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer.getGLFramebuffer());
    glViewport(0, 0, windowSize.x, windowSize.y);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const BlockMesh *blockMesh : blockMeshes.chunks) {
        if (blockMesh == nullptr) {
            continue;
        }

        client->spriteRenderer.renderMesh(blockMesh->frontMesh, shadowSpriteProgram, texture);
    }

    client->spriteRenderer.renderMesh(entities.spriteSystem.mesh, shadowSpriteProgram, texture);

    // Render main scene
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowSize.x, windowSize.y);

    engine->renderer.clearScreen(backgroundColor);

    for (const BlockMesh *blockMesh : blockMeshes.chunks) {
        if (blockMesh == nullptr) {
            continue;
        }

        client->spriteRenderer.renderMesh(blockMesh->backMesh, backSpriteProgram, texture);
        client->spriteRenderer.renderMesh(blockMesh->frontMesh, client->spriteProgram, texture);
    }

    client->spriteRenderer.renderMesh(entities.spriteSystem.mesh, client->spriteProgram, texture);
}

WorldRenderer::WorldRenderer(WorldScene &scene) :
    backSpriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment_back.glsl"),
    shadowSpriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment_shadow.glsl") {

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