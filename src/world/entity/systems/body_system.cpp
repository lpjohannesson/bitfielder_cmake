#include "body_system.h"
#include "entity_system_impl.h"
#include "../components/position_component.h"
#include "../components/body_component.h"
#include "engine/engine.h"

using namespace bf;

bool BodySystem::getCollisionX(const BodyMovement &movement, BodyCollision &collision) {
    if (movement.distance == 0.0f) {
        // Not moving
        return false;
    }

    const Box2
        &mover = movement.mover,
        &collider = movement.collider;

    if (mover.start.y >= collider.start.y + collider.size.y ||
        mover.start.y + mover.size.y <= collider.start.y)
    {
        // Other axis not aligned
        return false;
    }

    float wall;

    if (movement.distance < 0.0f) {
        // Move negative

        if (mover.start.x + mover.size.x < collider.start.x + collider.size.x) {
            // Already beyond
            return false;
        }

        wall = collider.start.x + collider.size.x;

        if (mover.start.x + movement.distance > wall) {
            // Not far enough
            return false;
        }
    }
    else {
        // Move positive
        
        if (mover.start.x > collider.start.x) {
            // Already beyond
            return false;
        }

        wall = collider.start.x - mover.size.x;

        if (mover.start.x + movement.distance < wall) {
            // Not far enough
            return false;
        }
    }

    // Success
    collision.endPosition = wall;

    return true;
}

bool BodySystem::getCollisionY(const BodyMovement &movement, BodyCollision &collision) {
    if (movement.distance == 0.0f) {
        // Not moving
        return false;
    }

    const Box2
        &mover = movement.mover,
        &collider = movement.collider;

    if (mover.start.x >= collider.start.x + collider.size.x ||
        mover.start.x + mover.size.x <= collider.start.x)
    {
        // Other axis not aligned
        return false;
    }

    float wall;

    if (movement.distance < 0.0f) {
        // Move negative

        if (mover.start.y + mover.size.y < collider.start.y + collider.size.y) {
            // Already beyond
            return false;
        }

        wall = collider.start.y + collider.size.y;

        if (mover.start.y + movement.distance > wall) {
            // Not far enough
            return false;
        }
    }
    else {
        // Move positive
        
        if (mover.start.y > collider.start.y) {
            // Already beyond
            return false;
        }

        wall = collider.start.y - mover.size.y;

        if (mover.start.y + movement.distance < wall) {
            // Not far enough
            return false;
        }
    }

    // Success
    collision.endPosition = wall;

    return true;
}

void BodySystem::update(World &world) {
    auto view = world.entities.registry.view<PositionComponent, BodyComponent>();

    for (auto [entity, position, body] : view.each()) {
        glm::vec2 distance = body.velocity * engine->getDeltaTime();
        
        BodyMovement movement;
        movement.mover = { position.position, body.size };
        movement.collider = { { 2.0f, 2.0f }, { 2.0f, 2.0f } };

        BodyCollision collision;
        bool collided;

        // Move X
        movement.distance = distance.x;
        collided = false;

        if (getCollisionX(movement, collision)) {
            collided = true;
            position.position.x = collision.endPosition;
        }

        if (collided) {
            body.velocity.x = 0;
        }
        else {
            position.position.x += distance.x;
        }
        
        // Move Y
        movement.distance = distance.y;
        collided = false;

        if (getCollisionY(movement, collision)) {
            collided = true;
            position.position.y = collision.endPosition;
        }

        if (collided) {
            body.velocity.y = 0;
        }
        else {
            position.position.y += distance.y;
        }
    }
}