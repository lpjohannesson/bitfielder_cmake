#pragma once
#include <FastNoiseLite.h>
#include "world/world.h"

namespace bf {
    class BlockMapGenerator {
    public:
        FastNoiseLite groundNoise, groundBackNoise, plantNoise;

        static float getBlendProgress(int y, int startY, int endY);
        static float blendSample(float sample, float blendProgress);

        float getGroundSample(glm::ivec2 position, float blendProgress);

        void generateChunk(BlockChunk &chunk, World &world);

        BlockMapGenerator();
    };
}