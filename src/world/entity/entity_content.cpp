#include "entity_content.h"
#include "world/world.h"
#include "components/position_component.h"
#include "components/animation_component.h"
#include "components/flip_component.h"
#include "components/aim_component.h"

using namespace bf;

void EntityContent::createPlayer(entt::entity player, World &world) {
    entt::registry &entityRegistry = world.entities.registry;

    entityRegistry.emplace<PositionComponent>(player, PositionComponent {});
    entityRegistry.emplace<AnimationComponent>(player, AnimationComponent {});
    entityRegistry.emplace<FlipComponent>(player, FlipComponent {});
    entityRegistry.emplace<AimComponent>(player, AimComponent {});
}

EntityContent::EntityContent(World &world) {
    world.entitySystems.push_back(&bodySystem);
}