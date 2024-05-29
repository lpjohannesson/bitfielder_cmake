#include "world_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "client/scenes/world_scene.h"
#include "client/client.h"

using namespace bf;

void WorldRenderer::updateSize(glm::ivec2 size) {
    viewTransform = glm::scale(glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f), glm::vec3(64.0f));
}

void WorldRenderer::render(const WorldScene &scene) {
    client->spriteRenderer.setTransform(viewTransform);

    map.render(scene);
    entities.render(scene);
}