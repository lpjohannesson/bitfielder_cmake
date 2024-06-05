#pragma once
#include "world/entity/systems/entity_system.h"

namespace bf {
    class WorldScene;

    class ClientEntitySystem : public EntitySystem {
    protected:
        WorldScene *scene;

    public:
        void loadScene(WorldScene &scene);
    };
}