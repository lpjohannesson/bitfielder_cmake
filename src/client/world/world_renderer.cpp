#include "world_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "client/scenes/world_scene.h"
#include "client/client.h"
#include "core/file_loader.h"

using namespace bf;

void WorldRenderer::loadTextureAtlas() {
    // Load all images to surface
    std::vector<std::string> texturePaths;
	FileLoader::getFilePaths("assets/textures/world", texturePaths);

	SDL_Surface *surface = textureAtlas.loadSurface(texturePaths);

    // Upload to texture and delete
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

    int blockStartX = glm::floor(screenBox.start.x - SHADOW_OFFSET);
    int blockEndX = glm::floor(screenBox.start.x + screenBox.size.x);

    BlockSample<BlockMesh> blockMeshes(scene.worldRenderer.map.map, blockStartX, blockEndX);

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getGLTexture());

    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowBuffer.texture.getGLTexture());

    client->spriteProgram.setTransform(viewTransform);
    backSpriteProgram.setTransform(viewTransform);
    shadowSpriteProgram.setTransform(shadowViewTransform);
    
    glm::ivec2 windowSize = engine->getWindowSize();

    // Render shadow to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer.getGLFramebuffer());
    glViewport(0, 0, windowSize.x, windowSize.y);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const BlockMesh *blockMesh : blockMeshes.chunks) {
        if (blockMesh == nullptr) {
            continue;
        }

        client->spriteRenderer.renderMesh(blockMesh->frontMesh, shadowSpriteProgram);
    }

    client->spriteRenderer.renderMesh(entities.spriteSystem.mesh, shadowSpriteProgram);

    // Render main scene
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowSize.x, windowSize.y);

    // TODO: Only render visible chunks
    for (const BlockMesh *blockMesh : blockMeshes.chunks) {
        if (blockMesh == nullptr) {
            continue;
        }

        client->spriteRenderer.renderMesh(blockMesh->backMesh, backSpriteProgram);
        client->spriteRenderer.renderMesh(blockMesh->frontMesh, client->spriteProgram);
    }

    client->spriteRenderer.renderMesh(entities.spriteSystem.mesh, client->spriteProgram);
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
}