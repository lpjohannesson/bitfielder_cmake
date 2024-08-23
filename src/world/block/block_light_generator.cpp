#include "block_light_generator.h"
#include "components/block_opaque_component.h"
#include "components/block_light_component.h"

using namespace bf;

#define REMOVE_LIGHT(GET_LIGHT, SET_LIGHT, CELL_QUEUE)\
    while (!removalQueue.empty()) {\
        BlockLightCell cell = removalQueue.front();\
        removalQueue.pop();\
        \
        BlockData *nextBlockData = BlockChunk::getWorldBlock(cell.position, world.map);\
        \
        if (nextBlockData == nullptr) {\
            continue;\
        }\
        \
        int nextLight = nextBlockData->GET_LIGHT();\
        \
        /* Skip if already removed */\
        if (nextLight == 0) {\
            continue;\
        }\
        \
        /* Requeue if brighter or spread */\
        if (cell.light <= nextLight) {\
            CELL_QUEUE.push({ cell.position, nextLight });\
        }\
        else {\
            for (glm::ivec2 offset : offsets) {\
                removalQueue.push({ cell.position + offset, nextLight });\
            }\
        }\
        \
        nextBlockData->SET_LIGHT(0);\
        resultBox.expandPoint(cell.position);\
    }

#define SPREAD_LIGHT(GET_LIGHT, SET_LIGHT, CELL_QUEUE) {\
    while (!CELL_QUEUE.empty()) {\
        BlockLightCell cell = CELL_QUEUE.front();\
        CELL_QUEUE.pop();\
        \
        BlockData *blockData = BlockChunk::getWorldBlock(cell.position, world.map);\
        \
        if (blockData == nullptr) {\
            continue;\
        }\
        \
        /* Skip if already bright enough */\
        if (cell.light <= blockData->GET_LIGHT()) {\
            continue;\
        }\
        \
        blockData->SET_LIGHT(cell.light);\
        resultBox.expandPoint(cell.position);\
        \
        int nextLight = cell.light - LIGHT_STEP;\
        \
        if (nextLight <= 0) {\
            continue;\
        }\
        \
        for (glm::ivec2 offset : offsets) {\
            CELL_QUEUE.push({ cell.position + offset, nextLight });\
        }\
    }\
}

#define UPDATE_SOURCE_LIGHT(GET_LIGHT, SET_LIGHT, CELL_QUEUE, LIGHT_CHANNEL)\
    removalQueue.push({ position, MAX_LIGHT + 1 });\
    REMOVE_LIGHT(GET_LIGHT, SET_LIGHT, CELL_QUEUE);\
    \
    if (hasLight) {\
        CELL_QUEUE.push({ position, LIGHT_CHANNEL });\
    }

bool BlockLightGenerator::isBlockOpaque(BlockData &blockData, World &world) {
    entt::entity block = world.blocks.getEntity(blockData.getFrontIndex());

    return world.blocks.registry.all_of<BlockOpaqueComponent>(block);
}

void BlockLightGenerator::queueNeighboringChunk(int x, BlockChunk *chunk, BlockLightQueue &queue) {
    if (chunk == nullptr) {
        return;
    }

    int blockX = chunk->getMapIndex() * BlockChunk::SIZE.x + x;

    for (int y = 0; y < BlockChunk::SIZE.y; y++) {
        BlockData &blockData = chunk->getBlock({ x, y });
        glm::ivec2 position = { blockX, y };

        queue.sun.push({ position, blockData.getSunlight() });
        queue.red.push({ position, blockData.getRedLight() });
        queue.green.push({ position, blockData.getGreenLight() });
        queue.blue.push({ position, blockData.getBlueLight() });

        blockData.getLightData() = 0;
    }
}

void BlockLightGenerator::updateSunlight(glm::ivec2 position, BlockLightQueue &queue, World &world, Box2i &resultBox) {
    BlockData *blockData = BlockChunk::getWorldBlock(position, world.map);

    // Skip if not full light
    if (blockData->getSunlight() < MAX_LIGHT) {
        return;
    }

    glm::ivec2 belowPosition = position;

    if (isBlockOpaque(*blockData, world)) {
        std::queue<BlockLightCell> removalQueue;

        // Remove light below
        while (true) {
            belowPosition.y++;

            BlockData *belowBlockData = BlockChunk::getWorldBlock(belowPosition, world.map);

            if (belowBlockData == nullptr) {
                break;
            }

            // Stop on non-max light
            if (belowBlockData->getSunlight() < MAX_LIGHT) {
                break;
            }

            // Queue above max to avoid brightness check
            removalQueue.push({ belowPosition, MAX_LIGHT + 1 });
        }

        REMOVE_LIGHT(getSunlight, setSunlight, queue.sun)
    }
    else {
        // Add light below
        while (true) {
            belowPosition.y++;

            BlockData *belowBlockData = BlockChunk::getWorldBlock(belowPosition, world.map);

            if (belowBlockData == nullptr) {
                break;
            }

            // Skip if already lit
            if (belowBlockData->getSunlight() == MAX_LIGHT) {
                break;
            }

            queue.sun.push({ belowPosition, MAX_LIGHT });
            belowBlockData->setSunlight(0);

            // Stop on opaque blocks
            if (isBlockOpaque(*belowBlockData, world)) {
                break;
            }
        }
    }
}

void BlockLightGenerator::spreadLight(BlockLightQueue &queue, World &world, Box2i &resultBox) {
    SPREAD_LIGHT(getSunlight, setSunlight, queue.sun);
    SPREAD_LIGHT(getRedLight, setRedLight, queue.red);
    SPREAD_LIGHT(getGreenLight, setGreenLight, queue.green);
    SPREAD_LIGHT(getBlueLight, setBlueLight, queue.blue);
}

void BlockLightGenerator::updateLight(glm::ivec2 position, World &world, Box2i &resultBox) {
    BlockData *blockData = BlockChunk::getWorldBlock(position, world.map);

    glm::ivec3 lightColor{};
    bool hasLight = false;

    entt::entity blocks[] = {
        world.blocks.getEntity(blockData->getFrontIndex()),
        world.blocks.getEntity(blockData->getBackIndex())
    };

    entt::registry &blocksRegistry = world.blocks.registry;

    for (entt::entity block : blocks) {
        if (blocksRegistry.all_of<BlockLightComponent>(block)) {
            lightColor = glm::max(lightColor, blocksRegistry.get<BlockLightComponent>(block).color);
            hasLight = true;
        }
    }
    
    BlockLightQueue queue;
    std::queue<BlockLightCell> removalQueue;

    updateSunlight(position, queue, world, resultBox);
    UPDATE_SOURCE_LIGHT(getRedLight, setRedLight, queue.red, lightColor.r);
    UPDATE_SOURCE_LIGHT(getGreenLight, setGreenLight, queue.green, lightColor.g);
    UPDATE_SOURCE_LIGHT(getBlueLight, setBlueLight, queue.blue, lightColor.b);

    spreadLight(queue, world, resultBox);
}

void BlockLightGenerator::generateChunk(BlockChunk &chunk, World &world) {
    int blockStartX = chunk.getMapIndex() * BlockChunk::SIZE.x;
    
    BlockLightQueue queue;

    // Fill top with full sunlight
    for (int x = 0; x < BlockChunk::SIZE.x; x++) {
        for (int y = 0; y < BlockChunk::SIZE.y; y++) {
            queue.sun.push({ { blockStartX + x, y }, MAX_LIGHT });

            if (isBlockOpaque(chunk.getBlock({ x, y }), world)) {
                break;
            }
        }
    }

    // Queue neighbours
    int mapIndex = chunk.getMapIndex();

    queueNeighboringChunk(BlockChunk::SIZE.x - 1, world.map.getChunk(mapIndex - 1), queue);
    queueNeighboringChunk(0, world.map.getChunk(mapIndex + 1), queue);

    Box2i resultBox;
    spreadLight(queue, world, resultBox);
}