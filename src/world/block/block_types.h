#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <entt/entt.hpp>

namespace bf {
	class BlockTypes {
	private:
		std::unordered_map<std::string, int> blockNames;

	public:
		std::vector<entt::entity> blocks;
		entt::registry registry;

		entt::entity getBlock(int index) const;
		int getBlockByName(const std::string name) const;

		entt::entity createBlock(const std::string name);
	};
}