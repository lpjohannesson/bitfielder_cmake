#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <entt/entt.hpp>

namespace bf {
	class BlockTypes {
	public:
		std::vector<entt::entity> blocks;
		entt::registry registry;

		entt::entity getBlock(int index) const;
		entt::entity createBlock(std::string name);
	};
}