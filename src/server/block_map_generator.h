#pragma once
#include "world/world.h"

namespace bf {
    class BlockMapGenerator {
    public:
        void generateChunk(World &world, BlockChunk &chunk);
    };
}