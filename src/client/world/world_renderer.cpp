#include "world_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "client/scenes/world_scene.h"
#include "client/client.h"

using namespace bf;

void WorldRenderer::updateSize(glm::ivec2 size) {
    glm::vec2 halfSize = glm::vec2(size) * 0.5f;

    viewTransform = glm::ortho(-halfSize.x, halfSize.x, halfSize.y, -halfSize.y);
    viewTransform = glm::scale(viewTransform, glm::vec3(64.0f));
}

void WorldRenderer::render(const WorldScene &scene) {
    entities.render(scene);

    glm::mat4 transform = viewTransform * scene.camera.getTransform();
    frontSpriteProgram.setTransform(transform);
    backSpriteProgram.setTransform(transform);

    const Texture &texture = scene.worldRenderer.textureAtlas.texture;

    // TODO: Only visible chunks
    for (const auto &[chunkIndex, chunk] : map.map.chunks) {
        client->spriteRenderer.renderMesh(
            chunk.backMesh, backSpriteProgram, texture);
    }

    for (const auto &[chunkIndex, chunk] : map.map.chunks) {
        client->spriteRenderer.renderMesh(
            chunk.frontMesh, frontSpriteProgram, texture);
    }

    client->spriteRenderer.renderMesh(
        entities.spriteSystem.mesh, frontSpriteProgram, texture);
}