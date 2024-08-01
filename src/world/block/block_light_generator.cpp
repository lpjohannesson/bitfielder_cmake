#include "block_light_generator.h"
#include "components/block_opaque_component.h"

using namespace bf;

bool BlockLightGenerator::isBlockOpaque(BlockData *blockData, World &world) {
    entt::entity block = world.blocks.getBlock(blockData->frontIndex);

    return world.blocks.registry.all_of<BlockOpaqueComponent>(block);
}

void BlockLightGenerator::queueNeighboringChunk(BlockChunk *chunk, int x, std::queue<BlockLightCell> &cellQueue) {
    if (chunk == nullptr) {
        return;
    }

    int blockStartX = chunk->getMapIndex() * BlockChunk::SIZE.x;

    for (int y = 0; y < BlockChunk::SIZE.y; y++) {
        BlockData *blockData = chunk->getBlock({ x, y });

        cellQueue.push({ { blockStartX + x, y }, blockData->light });
        blockData->light = 0;
    }
}

void BlockLightGenerator::updateLight(glm::ivec2 position, World &world) {
    // TODO: Chunk optimizations

    BlockData *blockData = BlockChunk::getWorldBlock(world.map, position);

    // Skip if not full light
    if (blockData->light < MAX_LIGHT) {
        return;
    }

    std::queue<BlockLightCell> cellQueue;

    glm::ivec2 belowPosition = position;

    if (isBlockOpaque(blockData, world)) {
        std::queue<BlockLightCell> removalQueue;

        // Remove light below
        while (true) {
            belowPosition.y++;

            BlockData *belowBlockData = BlockChunk::getWorldBlock(world.map, belowPosition);

            if (belowBlockData == nullptr) {
                break;
            }

            // Queue above max to avoid brightness check
            removalQueue.push({ belowPosition, MAX_LIGHT + 1 });

            // Stop on non-max light
            if (belowBlockData->light < MAX_LIGHT) {
                break;
            }
        }

        // Apply removal queue
        while (!removalQueue.empty()) {
            BlockLightCell cell = removalQueue.front();
            removalQueue.pop();

            BlockData *nextBlockData = BlockChunk::getWorldBlock(world.map, cell.position);

            if (nextBlockData == nullptr) {
                continue;
            }

            int nextLight = nextBlockData->light;
            
            // Skip if already removed
            if (nextLight == 0) {
                continue;
            }

            // Requeue if brighter or spread
            if (cell.light <= nextLight) {
                cellQueue.push({ cell.position, nextLight });
            }
            else {
                for (glm::ivec2 offset : offsets) {
                    removalQueue.push({ cell.position + offset, nextLight });
                }
            }

            nextBlockData->light = 0;
        }
    }
    else {
        // Add light below
        while (true) {
            belowPosition.y++;

            BlockData *belowBlockData = BlockChunk::getWorldBlock(world.map, belowPosition);

            if (belowBlockData == nullptr) {
                break;
            }

            cellQueue.push({ belowPosition, MAX_LIGHT });
            belowBlockData->light = 0;

            // Stop on opaque blocks
            if (isBlockOpaque(belowBlockData, world)) {
                break;
            }
        }
    }

    spreadLight(cellQueue, world);
}

void BlockLightGenerator::spreadLight(std::queue<BlockLightCell> &cellQueue, World &world) {
    // TODO: Chunk optimizations

    while (!cellQueue.empty()) {
        BlockLightCell cell = cellQueue.front();
        cellQueue.pop();

        BlockData *blockData = BlockChunk::getWorldBlock(world.map, cell.position);

        if (blockData == nullptr) {
            continue;
        }

        // Skip if already bright enough
        if (cell.light <= blockData->light) {
            continue;
        }

        blockData->light = cell.light;
        
        int nextLight = cell.light - LIGHT_STEP;

        if (nextLight <= 0) {
            continue;
        }

        for (glm::ivec2 offset : offsets) {
            cellQueue.push({ cell.position + offset, nextLight });
        }
    }
}

void BlockLightGenerator::generateChunk(BlockChunk &chunk, World &world) {
    int blockStartX = chunk.getMapIndex() * BlockChunk::SIZE.x;
    
    std::queue<BlockLightCell> cellQueue;

    // Re-queue neighbours
    int mapIndex = chunk.getMapIndex();

    queueNeighboringChunk(world.map.getChunk(mapIndex - 1), BlockChunk::SIZE.x - 1, cellQueue);
    queueNeighboringChunk(world.map.getChunk(mapIndex + 1), 0, cellQueue);

    // Fill top with full sunlight
    for (int x = 0; x < BlockChunk::SIZE.x; x++) {
        for (int y = 0; y < BlockChunk::SIZE.y; y++) {
            cellQueue.push({ { blockStartX + x, y }, MAX_LIGHT });

            if (isBlockOpaque(chunk.getBlock({ x, y }), world)) {
                break;
            }
        }
    }

    spreadLight(cellQueue, world);
}