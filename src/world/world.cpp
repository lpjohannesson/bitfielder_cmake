#include "world.h"
#include "block/components/block_attachable_component.h"
#include "block/block_light_generator.h"

using namespace bf;

bool World::isBlockAttachable(int index) {
    entt::entity block = blocks.getEntity(index);

    return blocks.registry.all_of<BlockAttachableComponent>(block);
}

bool World::isBlockPlaceable(glm::ivec2 position, bool onFrontLayer) {
    BlockData *blockData = BlockChunk::getWorldBlock(position, map);

    if (blockData == nullptr) {
        return false;
    }
    
    int frontIndex = blockData->getFrontIndex();
    int backIndex = blockData->getBackIndex();
    
    // Check for existing block
    int existingBlockIndex;

    if (onFrontLayer) {
        existingBlockIndex = frontIndex;
    }
    else {
        existingBlockIndex = backIndex;
    }

    if (existingBlockIndex != 0) {
        return false;
    }

    // Attach to back block
    if (onFrontLayer && isBlockAttachable(backIndex)) {
        return true;
    }

    // Attach to neighbours
    static constexpr glm::ivec2 neighborOffsets[] = { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };

    for (glm::ivec2 offset : neighborOffsets) {
        glm::ivec2 neighborPosition = position + offset;

        BlockData *neighborBlockData = BlockChunk::getWorldBlock(neighborPosition, map);

        if (neighborBlockData == nullptr) {
            continue;
        }

        if (isBlockAttachable(neighborBlockData->getFrontIndex())) {
            return true;
        }

        if (!onFrontLayer && isBlockAttachable(neighborBlockData->getBackIndex())) {
            return true;
        }
    }

    return false;
}

void World::updateBlock(glm::ivec2 position, Box2i &resultBox) {
    BlockLightGenerator::updateLight(position, *this, resultBox);
}

void World::update() {
    for (EntitySystem *system : entitySystems) {
        system->update(*this);
    }
}