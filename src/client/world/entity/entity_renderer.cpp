#include "entity_renderer.h"
#include "world/world.h"

using namespace bf;

void EntityRenderer::render(const WorldScene &scene) {
    spriteSystem.render(scene);
}