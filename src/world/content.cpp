#include "content.h"
#include "world.h"
#include "entity/components/position_component.h"
#include "entity/components/sprite_animation_component.h"
#include "entity/components/sprite_flip_component.h"
#include "entity/components/aim_component.h"
#include "block/components/block_attachable_component.h"
#include "block/components/block_collision_component.h"

using namespace bf;

void Content::createSolidBlock(World &world, entt::entity block) {
    world.blocks.registry.emplace<BlockAttachableComponent>(block, BlockAttachableComponent {});
    world.blocks.registry.emplace<BlockCollisionComponent>(block, BlockCollisionComponent {});
}

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
    createSolidBlock(world, dirtBlock);

    grassBlock = world.blocks.createBlock("grass");
    createSolidBlock(world, grassBlock);
    
    stoneBlock = world.blocks.createBlock("stone");
    createSolidBlock(world, stoneBlock);

    woodLogBlock = world.blocks.createBlock("wood_log");
    createSolidBlock(world, woodLogBlock);

    woodPlanksBlock = world.blocks.createBlock("wood_planks");
    createSolidBlock(world, woodPlanksBlock);

    leavesBlock = world.blocks.createBlock("leaves");
    createSolidBlock(world, leavesBlock);

    bushBlock = world.blocks.createBlock("bush");
    mushroomBlock = world.blocks.createBlock("mushroom");
    wheatBlock = world.blocks.createBlock("wheat");

    ironBlock = world.blocks.createBlock("iron_block");
    createSolidBlock(world, ironBlock);

    goldBlock = world.blocks.createBlock("gold_block");
    createSolidBlock(world, goldBlock);

    woolBlock = world.blocks.createBlock("wool_white");
    createSolidBlock(world, woolBlock);
}