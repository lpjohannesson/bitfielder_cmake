#include "content.h"
#include "world.h"
#include "entity/components/position_component.h"
#include "entity/components/body_component.h"

using namespace bf;

entt::entity Content::spawnPlayer(World &world, glm::vec2 position) {
    entt::registry &entityRegistry = world.entities.registry;

    entt::entity player = entityRegistry.create();

    entityRegistry.emplace<PositionComponent>(player, PositionComponent { position });

    return player;
}

void Content::loadContent(World &world) {
    // Add entity systems
    world.entities.addSystem(bodySystem);

    // Create blocks
    airBlock = world.blocks.createBlock();
    testBlock = world.blocks.createBlock();
}