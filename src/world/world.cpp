#include "world.h"
#include "block/components/block_attachable_component.h"
#include "block/block_light_generator.h"

using namespace bf;

bool World::isBlockAttachable(int index) {
    entt::entity block = blocks.getBlock(index);

    return blocks.registry.all_of<BlockAttachableComponent>(block);
}

bool World::isBlockPlaceable(glm::ivec2 position, bool onFrontLayer) {
    // Check back
    BlockData *blockData = BlockChunk::getWorldBlock(map, position);

    if (blockData != nullptr) {
        if (isBlockAttachable(blockData->backIndex)) {
            return true;
        }
    }

    // Check neighbours
    static constexpr glm::ivec2 neighborOffsets[] = { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };

    for (glm::ivec2 offset : neighborOffsets) {
        glm::vec2 neighborPosition = position + offset;

        BlockData *neighborBlockData = BlockChunk::getWorldBlock(map, neighborPosition);

        if (neighborBlockData == nullptr) {
            continue;
        }

        if (isBlockAttachable(neighborBlockData->frontIndex)) {
            return true;
        }

        if (!onFrontLayer) {
            if (isBlockAttachable(neighborBlockData->backIndex)) {
                return true;
            }
        }
    }

    return false;
}

void World::updateBlock(glm::ivec2 position) {
    BlockLightGenerator::updateLight(position, *this);
}

void World::update() {
    entities.update(*this);
}