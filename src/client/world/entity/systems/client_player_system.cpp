#include "client_player_system.h"
#include "world/entity/systems/entity_system_impl.h"
#include "../components/client_player_component.h"
#include "world/entity/components/body_component.h"
#include "core/game_time.h"
#include "core/direction.h"

using namespace bf;

void ClientPlayerSystem::update(World &world) {
    auto view = world.entities.registry.view<ClientPlayerComponent, BodyComponent>();

    float deltaTime = gameTime.getDeltaTime();

    // Input direction
    glm::vec2 movement = { 
        (float)client->clientInput.right.pressed - (float)client->clientInput.left.pressed,
        (float)client->clientInput.down.pressed - (float)client->clientInput.up.pressed
    };

    for (auto [entity, clientPlayer, body] : view.each()) {
        // Fall
        body.velocity.y += gravity * deltaTime;

        if (body.isOnFloor) {
            // Jump
            if (client->clientInput.jump.justPressed()) {
                body.velocity.y = -jumpImpulse;
                clientPlayer.jumpStopped = false;
            }
        }
        else {
            // Stop jump on release
            if (!clientPlayer.jumpStopped &&
                body.velocity.y < 0.0f &&
                client->clientInput.jump.justReleased()) {
                
                body.velocity.y *= jumpStop;
                clientPlayer.jumpStopped = true;
            }
        }

        // Move horizontally, friction only on the ground
        if (body.isOnFloor || movement.x != 0.0f) {
            Direction::targetAxis(body.velocity.x, movement.x * speed, acceleration * deltaTime);
        }
    }
}

ClientPlayerSystem::ClientPlayerSystem() {
    speed = 5.0f;
    acceleration = 24.0f;
    gravity = 20.0f;
    jumpImpulse = 9.0f;
    jumpStop = 0.65f;
}