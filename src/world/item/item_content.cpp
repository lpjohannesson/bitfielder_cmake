#include "item_content.h"
#include "world/world.h"
#include "world/registry/components/registry_index_component.h"
#include "components/item_block_component.h"

using namespace bf;

void ItemContent::createBlockItem(entt::entity item, entt::entity block, World &world) {
    // Assign block index to item
    int blockIndex = world.blocks.registry.get<RegistryIndexComponent>(block).index;

    world.items.registry.emplace<ItemBlockComponent>(item, ItemBlockComponent { blockIndex });
}

ItemContent::ItemContent(World &world) {
    BlockContent &blocks = world.content.blocks;

    dirt = world.items.createEntity("dirt");
    createBlockItem(dirt, blocks.dirt, world);

    stone = world.items.createEntity("stone");
    createBlockItem(stone, blocks.stone, world);

    woodLog = world.items.createEntity("wood_log");
    createBlockItem(woodLog, blocks.woodLog, world);

    woodPlanks = world.items.createEntity("wood_planks");
    createBlockItem(woodPlanks, blocks.woodPlanks, world);

    stick = world.items.createEntity("stick");
    createBlockItem(stick, blocks.stick, world);
    
    leaves = world.items.createEntity("leaves");
    createBlockItem(leaves, blocks.leaves, world);

    bush = world.items.createEntity("bush");
    createBlockItem(bush, blocks.bush, world);
    
    mushroom = world.items.createEntity("mushroom");
    createBlockItem(mushroom, blocks.mushroom, world);

    wheat = world.items.createEntity("wheat");
    createBlockItem(wheat, blocks.wheat, world);

    torch = world.items.createEntity("torch");
    createBlockItem(torch, blocks.torch, world);

    iron = world.items.createEntity("block_iron");
    createBlockItem(iron, blocks.iron, world);
}