#include "content.h"
#include "world.h"
#include "entity/components/position_component.h"
#include "entity/components/body_component.h"

using namespace bf;

void Content::createPlayer(entt::entity player, World &world, glm::vec2 position) {
    entt::registry &entityRegistry = world.entities.registry;

    entityRegistry.emplace<PositionComponent>(player, PositionComponent { position });
}

void Content::loadContent(World &world) {
    // Add entity systems
    world.entities.addSystem(bodySystem);

    // Create blocks
    airBlock = world.blocks.createBlock();
    testBlock = world.blocks.createBlock();
}