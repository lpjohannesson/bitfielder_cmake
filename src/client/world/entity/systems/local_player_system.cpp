#include "local_player_system.h"
#include "world/entity/systems/entity_system_impl.h"
#include "client/scenes/world_scene.h"
#include "../components/local_player_component.h"
#include "../components/sprite_component.h"
#include "../components/sprite_animator_component.h"
#include "../systems/sprite_animator_system.h"
#include "world/entity/components/body_component.h"
#include "core/game_time.h"
#include "core/direction.h"

using namespace bf;

void LocalPlayerSystem::update(World &world) {
    float deltaTime = gameTime.getDeltaTime();

    // Input direction
    glm::vec2 movement = {
        (float)client->clientInput.right.pressed - (float)client->clientInput.left.pressed,
        (float)client->clientInput.down.pressed - (float)client->clientInput.up.pressed
    };

    auto view = world.entities.registry.view<LocalPlayerComponent, BodyComponent, SpriteComponent, SpriteAnimatorComponent>();

    for (auto [entity, localPlayer, body, sprite, spriteAnimator] : view.each()) {
        // Update floor timer
        if (body.isOnFloor) {
            localPlayer.floorTime = maxFloorTime;
        }
        else {
            localPlayer.floorTime = glm::max(0.0f, localPlayer.floorTime - deltaTime);
        }

        bool canJump = localPlayer.floorTime > 0.0f;

        if (canJump) {
            // Jump
            if (client->clientInput.jump.justPressed()) {
                body.velocity.y = -jumpImpulse;

                localPlayer.jumpStopped = false;
                localPlayer.floorTime = 0;
            }
        }
        else {
            // Stop jump on release
            if (!localPlayer.jumpStopped &&
                body.velocity.y < 0.0f &&
                client->clientInput.jump.justReleased()) {
                
                body.velocity.y *= jumpStop;
                localPlayer.jumpStopped = true;
            }
        }

        // Fall
        body.velocity.y += gravity * deltaTime;

        // Flip sprite
        if (movement.x != 0.0f) {
            sprite.flipX = movement.x < 0.0f;
        }

        // Animate
        ClientContent &clientContent = scene->clientContent;
        SpriteAnimation *animation;

        if (body.isOnFloor) {
            if (body.velocity.x == 0.0f || movement.x == 0.0f) {
                animation = &clientContent.playerIdle;
            }
            else {
                if (glm::sign(body.velocity.x) == movement.x) {
                    animation = &clientContent.playerWalk;
                }
                else {
                    animation = &clientContent.playerSlide;
                }
            }
        }
        else {
            animation = &clientContent.playerJump;
        }

        SpriteAnimatorSystem::playAnimation(spriteAnimator, *animation);

        // Move horizontally, friction only on the ground
        if (body.isOnFloor || movement.x != 0.0f) {
            Direction::targetAxis(body.velocity.x, movement.x * speed, acceleration * deltaTime);
        }
    }
}

void LocalPlayerSystem::loadContent(WorldScene &scene) {
    this->scene = &scene;
}

LocalPlayerSystem::LocalPlayerSystem() {
    speed = 5.0f;
    acceleration = 24.0f;
    gravity = 20.0f;
    jumpImpulse = 10.0f;
    jumpStop = 0.55f;
    maxFloorTime = 0.1f;
}