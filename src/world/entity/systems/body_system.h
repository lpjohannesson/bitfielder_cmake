#pragma once
#include "entity_system.h"
#include "core/box2.h"

namespace bf {
    struct BodyMovement {
        float distance;
        Box2 mover, collider;
    };

    struct BodyCollision {
        float endPosition;
    };

	class BodySystem : public EntitySystem {
    private:
        bool getCollisionX(const BodyMovement &movement, BodyCollision &collision);
        bool getCollisionY(const BodyMovement &movement, BodyCollision &collision);

    public:
        void update(World &world) override;
	};
}