#include "block_content.h"
#include "world/world.h"
#include "components/block_attachable_component.h"
#include "components/block_collision_component.h"
#include "components/block_opaque_component.h"
#include "components/block_one_way_component.h"
#include "components/block_light_component.h"

using namespace bf;

void BlockContent::createSolid(World &world, entt::entity block) {
    entt::registry &blocksRegistry = world.blocks.registry;

    blocksRegistry.emplace<BlockAttachableComponent>(block, BlockAttachableComponent {});
    blocksRegistry.emplace<BlockCollisionComponent>(block, BlockCollisionComponent {});
    blocksRegistry.emplace<BlockOpaqueComponent>(block, BlockOpaqueComponent {});
}

BlockContent::BlockContent(World &world) {
    entt::registry &blocksRegistry = world.blocks.registry;

    air = world.blocks.createEntity("air");

    dirt = world.blocks.createEntity("dirt");
    createSolid(world, dirt);

    grass = world.blocks.createEntity("grass");
    createSolid(world, grass);
    
    stone = world.blocks.createEntity("stone");
    createSolid(world, stone);

    woodLog = world.blocks.createEntity("wood_log");
    createSolid(world, woodLog);

    woodPlanks = world.blocks.createEntity("wood_planks");
    createSolid(world, woodPlanks);

    stick = world.blocks.createEntity("stick");
    blocksRegistry.emplace<BlockAttachableComponent>(stick, BlockAttachableComponent {});
    blocksRegistry.emplace<BlockCollisionComponent>(stick, BlockCollisionComponent {});
    blocksRegistry.emplace<BlockOneWayComponent>(stick, BlockOneWayComponent {});

    leaves = world.blocks.createEntity("leaves");
    createSolid(world, leaves);

    bush = world.blocks.createEntity("bush");

    mushroom = world.blocks.createEntity("mushroom");
    blocksRegistry.emplace<BlockLightComponent>(mushroom, BlockLightComponent { { 15, 11, 8 } });

    wheat = world.blocks.createEntity("wheat");

    iron = world.blocks.createEntity("block_iron");
    createSolid(world, iron);

    gold = world.blocks.createEntity("block_gold");
    createSolid(world, gold);

    wool = world.blocks.createEntity("wool_white");
    createSolid(world, wool);
}