#pragma once
#include "world/entity/systems/entity_system.h"
#include "gfx/sprite/animation/sprite_frames.h"

namespace bf {
    class WorldScene;

	class LocalPlayerSystem : public EntitySystem {
    public:
        float speed, acceleration, gravity, jumpImpulse, jumpStop, maxFloorTime;
        WorldScene *scene;

        void loadContent(WorldScene &scene);

        void update(World &world) override;

        LocalPlayerSystem();
	};
}