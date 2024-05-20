#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <entt/entt.hpp>

namespace bf {
	class BlockTypes {
	private:
		std::vector<entt::entity> blocks;

	public:
		entt::registry registry;

		entt::entity getBlock(int index) const;
		entt::entity createBlock();

		inline BlockTypes() {

		}
	};
}