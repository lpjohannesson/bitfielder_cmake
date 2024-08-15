#pragma once
#include <unordered_map>
#include <entt/entt.hpp>

namespace bf {
	class Entities {
    public:
        std::unordered_map<int, entt::entity> entityIDs;
        int nextID = 0;

        entt::registry registry;

        bool getEntity(int id, entt::entity &entity);

        entt::entity spawnEntity(int id);
        entt::entity spawnEntity();

        void despawnEntity(int id);
    };
}