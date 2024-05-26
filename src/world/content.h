#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "entity/systems/body_system.h"

namespace bf {
    class World;

    class Content {
    public:
        BodySystem bodySystem;

        entt::entity airBlock, testBlock;

        entt::entity spawnPlayer(World &world, glm::vec2 position);

        void loadContent(World &world);
    };
}