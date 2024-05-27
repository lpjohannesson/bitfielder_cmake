#pragma once
#include <unordered_map>
#include <entt/entt.hpp>
#include "systems/entity_system.h"

namespace bf {
	class Entities {
    public:
        std::unordered_map<int, entt::entity> entityIDs;
        int nextID = 0;

        entt::registry registry;
        std::vector<EntitySystem*> systems;

        bool getEntity(int id, entt::entity &entity);

        entt::entity spawnEntity(int id);
        entt::entity spawnEntity();

        void despawnEntity(int id);

        void addSystem(EntitySystem &system);

        void update(World &world);
    };
}