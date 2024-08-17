#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <entt/entt.hpp>

namespace bf {
	class EntityRegistry {
	private:
		std::unordered_map<std::string, int> entityNames;

	public:
		entt::registry registry;
		std::vector<entt::entity> entities;

		entt::entity getEntity(int index) const;
		int getEntityByName(const std::string name) const;

		entt::entity createEntity(const std::string name);
	};
}