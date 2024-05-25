#pragma once
#include "entity_system.h"
#include "core/box2.h"
#include "../components/position_component.h"
#include "../components/body_component.h"

namespace bf {
    struct BodyMovement {
        Box2 mover, collider;
    };

	class BodySystem : public EntitySystem {
    private:
        bool getCollisionX(const BodyMovement &movement, float &endPosition);
        bool getCollisionY(const BodyMovement &movement, float &endPosition);

        void moveX(World &world, glm::vec2 &position, BodyComponent &body);
        void moveY(World &world, glm::vec2 &position, BodyComponent &body);

    public:
        void update(World &world) override;
	};
}