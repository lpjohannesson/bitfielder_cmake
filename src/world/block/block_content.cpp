#include "block_content.h"
#include "world/world.h"
#include "components/block_attachable_component.h"
#include "components/block_collision_component.h"
#include "components/block_opaque_component.h"
#include "components/block_one_way_component.h"

using namespace bf;

void BlockContent::createSolid(World &world, entt::entity block) {
    world.blocks.registry.emplace<BlockAttachableComponent>(block, BlockAttachableComponent {});
    world.blocks.registry.emplace<BlockCollisionComponent>(block, BlockCollisionComponent {});
    world.blocks.registry.emplace<BlockOpaqueComponent>(block, BlockOpaqueComponent {});
}

BlockContent::BlockContent(World &world) {
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
    world.blocks.registry.emplace<BlockAttachableComponent>(stick, BlockAttachableComponent {});
    world.blocks.registry.emplace<BlockCollisionComponent>(stick, BlockCollisionComponent {});
    world.blocks.registry.emplace<BlockOneWayComponent>(stick, BlockOneWayComponent {});

    leaves = world.blocks.createEntity("leaves");
    createSolid(world, leaves);

    bush = world.blocks.createEntity("bush");
    mushroom = world.blocks.createEntity("mushroom");
    wheat = world.blocks.createEntity("wheat");

    iron = world.blocks.createEntity("block_iron");
    createSolid(world, iron);

    gold = world.blocks.createEntity("block_gold");
    createSolid(world, gold);

    wool = world.blocks.createEntity("wool_white");
    createSolid(world, wool);
}