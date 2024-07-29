#include "block_map_generator.h"
#include <glm/gtx/easing.hpp>
#include "world/block/block_light_generator.h"

using namespace bf;

float BlockMapGenerator::getBlendProgress(int y, int startY, int endY) {
    float progress = (float)(y - startY) / (endY - startY);

    return progress;
}

float BlockMapGenerator::blendSample(float sample, float blendProgress) {
    if (blendProgress < 0.5f) {
        return glm::lerp(0.0f, sample, blendProgress * 2.0f);
    }

    return glm::lerp(sample, 1.0f, (blendProgress - 0.5f) * 2.0f);
}

float BlockMapGenerator::getGroundSample(glm::ivec2 position, float blendProgress) {
    float sample = groundNoise.GetNoise((float)position.x, (float)position.y);
    
    return blendSample(sample, blendProgress);
}

void BlockMapGenerator::generateChunk(BlockChunk &chunk, World &world) {
    const int
        groundStart = 128,
        groundEnd = groundStart + 16,
        groundBackStart = groundStart + 6,
        groundBackEnd = groundEnd - 4,
        stoneStart = groundEnd + 8,
        stoneEnd = stoneStart + 16;

    // TODO: Add system for block type IDs
    int
        airIndex = 0,
        dirtIndex = 1,
        grassIndex = 2,
        bushIndex = 7,
        mushroomIndex = 8;
    
    int blockX = chunk.getMapIndex() * BlockChunk::SIZE.x;

    // Create ground
    for (int y = groundStart; y < groundEnd; y++) {
        float blendProgress = getBlendProgress(y, groundStart, groundEnd);

        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            float sample = getGroundSample({ blockX + x, y }, blendProgress);
            
            if (sample > 0.35f) {
                BlockData *block = chunk.getBlock({ x, y });
                block->frontIndex = dirtIndex;
            }
        }
    }

    for (int y = groundEnd; y < BlockChunk::SIZE.y; y++) {
        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            BlockData *block = chunk.getBlock({ x, y });
            block->frontIndex = dirtIndex;
        }
    }

    // Add grass and plants
    for (int y = groundStart; y < groundEnd; y++) {
        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            BlockData *block = chunk.getBlock({ x, y });
            BlockData *aboveBlock = chunk.getBlock({ x, y - 1 });

            if (block->frontIndex == dirtIndex && aboveBlock->frontIndex == airIndex) {
                block->frontIndex = grassIndex;

                float plantSample = plantNoise.GetNoise((float)(blockX + x), (float)y);

                if (plantSample > 0.75f) {
                    if (plantSample > 0.85f) {
                        aboveBlock->frontIndex = bushIndex;
                    }
                    else {
                        aboveBlock->frontIndex = mushroomIndex;
                    }
                }
            }
        }
    }

    // Add back layer
    for (int x = 0; x < BlockChunk::SIZE.x; x++) {
        float height = groundBackNoise.GetNoise((float)(blockX + x), 0.0f) * 0.5f + 0.5f;
        int start = (int)glm::round(groundBackStart + (groundBackEnd - groundBackStart) * height);

        for (int y = start; y < groundBackEnd; y++) {
            BlockData *block = chunk.getBlock({ x, y });
            block->backIndex = dirtIndex;
        }
    }

    for (int y = groundBackEnd; y < BlockChunk::SIZE.y; y++) {
        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            BlockData *block = chunk.getBlock({ x, y });
            block->backIndex = dirtIndex;
        }
    }

    BlockLightGenerator::generateChunk(chunk, world.map);
}

BlockMapGenerator::BlockMapGenerator() {
    srand((int)time(NULL));
    int seed = rand();

    groundNoise.SetSeed(seed);
    groundNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    groundNoise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Manhattan);
    groundNoise.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
    groundNoise.SetFrequency(0.25f);

    groundBackNoise.SetSeed(seed);
    groundBackNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    groundBackNoise.SetFrequency(0.05f);

    plantNoise.SetSeed(seed);
    plantNoise.SetNoiseType(FastNoiseLite::NoiseType_Value);
    plantNoise.SetFrequency(1.0f);
}