#pragma once
#include <glm/glm.hpp>
#include "block_map_element.h"

namespace bf {
	class BlockData {
	private:
		uint16_t frontIndex, backIndex;
		uint16_t light;

	public:
		int getFrontIndex() const { return frontIndex; }
		void setFrontIndex(int value) { frontIndex = value; }

		int getBackIndex() const { return backIndex; }
		void setBackIndex(int value) { backIndex = value; }

		int getSunlight() const { return light; }
		void setSunlight(int value) { light = value; }

		uint16_t &getLightData() { return light; }
	};

	template <typename T>
	class BlockSample;

	template <typename T>
	class BlockMap;

	class BlockChunk : public BlockMapElement {
	public:
		static constexpr glm::ivec2 SIZE{ 16, 256 };

		BlockData data[SIZE.y][SIZE.x];

		static int getChunkIndex(int blockX);
		static glm::ivec2 worldToChunk(glm::ivec2 position, int chunkIndex);

		static BlockData *getWorldBlock(BlockMap<BlockChunk> &map, glm::ivec2 position);
		static BlockData *getSampleBlock(BlockSample<BlockChunk> &sample, glm::ivec2 position);

		BlockData &getBlock(glm::ivec2 position);
		BlockData *getBlockChecked(glm::ivec2 position);

		BlockChunk(int mapIndex);
	};
}