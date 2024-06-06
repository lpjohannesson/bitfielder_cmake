#include "world_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "client/scenes/world_scene.h"
#include "client/client.h"

using namespace bf;

void WorldRenderer::updateTransforms(const WorldScene &scene) {
    viewTransform = windowTransform * scene.camera.getTransform();
    shadowViewTransform = viewTransform * shadowTransform;

    // Update screen box
    glm::mat4 viewTransformInverse = glm::inverse(viewTransform);

    screenBox.start = glm::vec2(viewTransformInverse * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f));
    screenBox.size = glm::vec2(viewTransformInverse * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f)) - screenBox.start; 
}

void WorldRenderer::updateSize(glm::ivec2 size, const WorldScene &scene) {
    glm::vec2 halfSize = glm::floor(glm::vec2(size) * 0.5f);

    windowTransform = glm::ortho(-halfSize.x, halfSize.x, halfSize.y, -halfSize.y);

    shadowBuffer.updateSize(size);

    updateTransforms(scene);
}

void WorldRenderer::render(const WorldScene &scene) {
    entities.render(scene);

    updateTransforms(scene);

    // Get visible chunks
    Box2 screenBox = scene.worldRenderer.getScreenBox();

    int blockStartX = glm::floor(screenBox.start.x);
    int blockEndX = glm::floor(screenBox.start.x + screenBox.size.x);

    BlockSample<BlockMesh> blockMeshes(scene.worldRenderer.map.map, blockStartX, blockEndX);

    // Bind textures
    const Texture &texture = scene.worldRenderer.textureAtlas.texture;

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getGLTexture());

    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowBuffer.texture.getGLTexture());

    frontSpriteProgram.setTransform(viewTransform);
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
        client->spriteRenderer.renderMesh(blockMesh->frontMesh, frontSpriteProgram);
    }

    client->spriteRenderer.renderMesh(entities.spriteSystem.mesh, frontSpriteProgram);
}

WorldRenderer::WorldRenderer(WorldScene &scene) :
    frontSpriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"),
    backSpriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment_back.glsl"),
    shadowSpriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment_shadow.glsl") {

    // Set shadow offset
    shadowTransform = glm::translate(glm::mat4(1.0f), glm::vec3(glm::vec2(2.0f) / 16.0f, 0.0f));

    // Bind textures
    GLint
        frontTextureLocation = glGetUniformLocation(frontSpriteProgram.getGLProgram(), "fTexture"),
        backTextureLocation = glGetUniformLocation(backSpriteProgram.getGLProgram(), "fTexture"),
        backShadowTextureLocation = glGetUniformLocation(backSpriteProgram.getGLProgram(), "fShadowTexture"),
        shadowTextureLocation = glGetUniformLocation(shadowSpriteProgram.getGLProgram(), "fTexture");
	
    glUseProgram(frontSpriteProgram.getGLProgram());
    glUniform1i(frontTextureLocation, 0);

    glUseProgram(backSpriteProgram.getGLProgram());
    glUniform1i(backTextureLocation, 0);
    glUniform1i(backShadowTextureLocation, 1);

    glUseProgram(shadowSpriteProgram.getGLProgram());
    glUniform1i(shadowTextureLocation, 0);
}