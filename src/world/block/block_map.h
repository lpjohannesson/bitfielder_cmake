#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

namespace bf {
	template <typename T>
	class BlockMap {
	private:
		T *lastChunk;
		int lastChunkIndex;
		bool lastChunkValid = false;

	public:
		std::unordered_map<int, T> chunks;

		inline T *getChunk(int index) {
			// Return if same as previous
			if (lastChunkValid && index == lastChunkIndex) {
				return lastChunk;
			}

			// Find chunk by index
			auto foundChunk = chunks.find(index);

			if (foundChunk == chunks.end()) {
				lastChunk = nullptr;
			}
			else {
				lastChunk = &foundChunk->second;
			}

			lastChunkIndex = index;
			lastChunkValid = true;

			return lastChunk;
		}

		inline T &createChunk(int index) {
			// Construct chunk with index parameter
			chunks.emplace(index, index);
			return chunks.at(index);
		}
	};
}