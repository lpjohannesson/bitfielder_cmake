#include "world_renderer.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace bf;

void WorldRenderer::updateSize(glm::ivec2 size) {
    viewTransform = glm::scale(glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f), glm::vec3(32.0f));
}

void WorldRenderer::render(const World &world) {
    spriteRenderer.setTransform(getViewTransform());

    map.render(*this, world);
}