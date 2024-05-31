#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

namespace bf {
	template <typename T>
	class BlockMap {
	public:
		std::unordered_map<int, T> chunks;

		inline T *getChunk(int index) const {
			// Find chunk by index
			auto foundChunk = chunks.find(index);

			if (foundChunk == chunks.end()) {
				return nullptr;
			}

			return (T*)&foundChunk->second;
		}

		inline T &createChunk(int index) {
			// Construct chunk with index parameter
			chunks.emplace(index, index);
			return chunks.at(index);
		}
	};
}