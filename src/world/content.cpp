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
    stoneBlock = world.blocks.createBlock("stone");
    woodLogBlock = world.blocks.createBlock("wood_log");
    woodPlanksBlock = world.blocks.createBlock("wood_planks");
    leavesBlock = world.blocks.createBlock("leaves");
    bushBlock = world.blocks.createBlock("bush");
    mushroomBlock = world.blocks.createBlock("mushroom");
    ironBlockBlock = world.blocks.createBlock("iron_block");
    goldBlockBlock = world.blocks.createBlock("gold_block");

    world.blocks.createBlock("wool_white");
    world.blocks.createBlock("wool_black");
    world.blocks.createBlock("wool_red");
    world.blocks.createBlock("wool_green");
    world.blocks.createBlock("wool_blue");
    world.blocks.createBlock("wool_cyan");
    world.blocks.createBlock("wool_magenta");
    world.blocks.createBlock("wool_yellow");
}