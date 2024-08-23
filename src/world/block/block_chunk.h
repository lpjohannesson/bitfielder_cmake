#pragma once
#include <glm/glm.hpp>
#include "block_map_element.h"

namespace bf {
	class BlockData {
	private:
		static constexpr int SHIFT_SUN = 0, SHIFT_RED = 4, SHIFT_GREEN = 8, SHIFT_BLUE = 12;

		uint16_t frontIndex, backIndex;
		uint16_t light;

		inline int getLight(int shift) const { return (light >> shift) & 0x0f; }
		inline void setLight(int value, int shift) { light = light & ~(0x0f << shift) | value << shift; }

	public:
		inline int getFrontIndex() const { return frontIndex; }
		inline void setFrontIndex(int value) { frontIndex = value; }

		inline int getBackIndex() const { return backIndex; }
		inline void setBackIndex(int value) { backIndex = value; }

		inline int getSunlight() const { return getLight(SHIFT_SUN); }
		inline void setSunlight(int value) { setLight(value, SHIFT_SUN); }

		inline int getRedLight() const { return getLight(SHIFT_RED); }
		inline void setRedLight(int value) { setLight(value, SHIFT_RED); }

		inline int getGreenLight() const { return getLight(SHIFT_GREEN); }
		inline void setGreenLight(int value) { setLight(value, SHIFT_GREEN); }

		inline int getBlueLight() const { return getLight(SHIFT_BLUE); }
		inline void setBlueLight(int value) { setLight(value, SHIFT_BLUE); }

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

		static BlockData *getWorldBlock(glm::ivec2 position, BlockMap<BlockChunk> &map);
		static BlockData *getSampleBlock(glm::ivec2 position, BlockSample<BlockChunk> &sample);

		BlockData &getBlock(glm::ivec2 position);
		BlockData *getBlockChecked(glm::ivec2 position);

		BlockChunk(int mapIndex);
	};
}