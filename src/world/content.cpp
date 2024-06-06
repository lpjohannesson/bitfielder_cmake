#include "content.h"
#include "world.h"
#include "entity/components/position_component.h"
#include "entity/components/sprite_animation_component.h"
#include "entity/components/sprite_flip_component.h"
#include "entity/components/aim_component.h"

using namespace bf;

void Content::createPlayer(entt::entity player, World &world) {
    entt::registry &entityRegistry = world.entities.registry;

    entityRegistry.emplace<PositionComponent>(player, PositionComponent {});
    entityRegistry.emplace<SpriteAnimationComponent>(player, SpriteAnimationComponent {});
    entityRegistry.emplace<SpriteFlipComponent>(player, SpriteFlipComponent {});
    entityRegistry.emplace<AimComponent>(player, AimComponent {});
}

Content::Content(World &world) {
    // Add entity systems
    world.entities.addSystem(bodySystem);

    // Create blocks
    airBlock = world.blocks.createBlock("air");
    dirtBlock = world.blocks.createBlock("dirt");
    grassBlock = world.blocks.createBlock("grass");
    woodBlock = world.blocks.createBlock("wood");
    goldBlock = world.blocks.createBlock("gold");
    woolBlock = world.blocks.createBlock("wool");
}