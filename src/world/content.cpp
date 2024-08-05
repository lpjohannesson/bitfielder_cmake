#include "content.h"
#include "world.h"
#include "entity/components/position_component.h"
#include "entity/components/sprite_animation_component.h"
#include "entity/components/sprite_flip_component.h"
#include "entity/components/aim_component.h"
#include "block/components/block_attachable_component.h"
#include "block/components/block_collision_component.h"
#include "block/components/block_opaque_component.h"

using namespace bf;

void Content::createSolidBlock(World &world, entt::entity block) {
    world.blocks.registry.emplace<BlockAttachableComponent>(block, BlockAttachableComponent {});
    world.blocks.registry.emplace<BlockCollisionComponent>(block, BlockCollisionComponent {});
    world.blocks.registry.emplace<BlockOpaqueComponent>(block, BlockOpaqueComponent {});
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
    airBlock = world.blocks.createEntity("air");

    dirtBlock = world.blocks.createEntity("dirt");
    createSolidBlock(world, dirtBlock);

    grassBlock = world.blocks.createEntity("grass");
    createSolidBlock(world, grassBlock);
    
    stoneBlock = world.blocks.createEntity("stone");
    createSolidBlock(world, stoneBlock);

    woodLogBlock = world.blocks.createEntity("wood_log");
    createSolidBlock(world, woodLogBlock);

    woodPlanksBlock = world.blocks.createEntity("wood_planks");
    createSolidBlock(world, woodPlanksBlock);

    leavesBlock = world.blocks.createEntity("leaves");
    createSolidBlock(world, leavesBlock);

    bushBlock = world.blocks.createEntity("bush");
    mushroomBlock = world.blocks.createEntity("mushroom");
    wheatBlock = world.blocks.createEntity("wheat");

    ironBlock = world.blocks.createEntity("block_iron");
    createSolidBlock(world, ironBlock);

    goldBlock = world.blocks.createEntity("block_gold");
    createSolidBlock(world, goldBlock);

    woolBlock = world.blocks.createEntity("wool_white");
    createSolidBlock(world, woolBlock);
}