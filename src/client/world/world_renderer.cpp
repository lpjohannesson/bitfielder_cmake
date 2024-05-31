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
    client->spriteRenderer.setTransform(viewTransform * scene.camera.getTransform());

    map.render(scene);
    entities.render(scene);
}