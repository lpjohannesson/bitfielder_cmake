#include "content.h"
#include "world/world.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/body_component.h"

using namespace bf;

void Content::loadContent(World &world) {
    // Add entity systems
    world.entities.addSystem(bodySystem);

    // Create blocks
    airBlock = world.blocks.createBlock();
    testBlock = world.blocks.createBlock();

    entt::registry &entityRegistry = world.entities.registry;

    // Create player
    player = entityRegistry.create();

    entityRegistry.emplace<PositionComponent>(player, PositionComponent { });
    entityRegistry.emplace<BodyComponent>(player, BodyComponent { { 1.0f, 1.0f } });
}