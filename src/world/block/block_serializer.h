#pragma once
#include "core/packet.h"
#include "world/world.h"

namespace bf {
	class BlockSerializer {
    private:
        static void createPaletteIndex(
            int blockIndex,
            World &world,
            std::vector<std::string> &paletteList,
            std::unordered_map<int, uint16_t> &paletteMap);

	public:
		static void writeChunk(BlockChunk &chunk, World &world, Packet &result);
        static void readChunk(Packet &input, BlockChunk &chunk, World &world);
	};
}