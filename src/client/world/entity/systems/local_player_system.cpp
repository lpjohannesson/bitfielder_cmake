#include "local_player_system.h"
#include "world/entity/systems/entity_system_impl.h"
#include "client/scenes/world_scene.h"
#include "../components/local_player_component.h"
#include "../components/sprite_component.h"
#include "../components/sprite_animator_component.h"
#include "../systems/sprite_animator_system.h"
#include "world/entity/components/body_component.h"
#include "world/entity/components/sprite_flip_component.h"
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

    auto view = world.entities.registry.view<
        LocalPlayerComponent,
        PositionComponent,
        BodyComponent,
        SpriteComponent,
        SpriteFlipComponent,
        SpriteAnimationComponent,
        SpriteAnimatorComponent>();

    for (auto [entity, localPlayer, position, body, sprite, spriteFlip, spriteAnimation, spriteAnimator] : view.each()) {
        // Flip sprite
        if (movement.x != 0.0f) {
            spriteFlip.flipX = movement.x < 0.0f;
        }

        // Animate
        PlayerAnimation animationIndex;

        if (body.isOnFloor) {
            if (body.velocity.x == 0.0f || movement.x == 0.0f) {
                animationIndex = PlayerAnimation::IDLE;
            }
            else {
                if (glm::sign(body.velocity.x) == movement.x) {
                    animationIndex = PlayerAnimation::WALK;
                }
                else {
                    animationIndex = PlayerAnimation::SLIDE;
                }
            }
        }
        else {
            if (body.velocity.y < 0.0f) {
                animationIndex = PlayerAnimation::JUMP;
            }
            else {
                animationIndex = PlayerAnimation::FALL;
            }
        }

        localPlayer.spriteAnimationDirty = SpriteAnimatorSystem::playAnimation(spriteAnimator, spriteAnimation, (int)animationIndex);

        // Move horizontally, friction only on the ground
        if (body.isOnFloor || movement.x != 0.0f) {
            Direction::targetAxis(body.velocity.x, movement.x * speed, acceleration * deltaTime);
        }

        // Update jump timer, allowing early jumps
        if (client->clientInput.jump.justPressed()) {
            localPlayer.jumpTime = maxJumpTime;
        }
        else {
            localPlayer.jumpTime = glm::max(0.0f, localPlayer.jumpTime - deltaTime);
        }

        // Update floor timer, allowing late jumps
        if (body.isOnFloor) {
            localPlayer.floorTime = maxFloorTime;
        }
        else {
            localPlayer.floorTime = glm::max(0.0f, localPlayer.floorTime - deltaTime);
        }

        bool canJump = localPlayer.floorTime > 0.0f;

        if (canJump) {
            // Jump
            if (localPlayer.jumpTime > 0.0f) {
                body.velocity.y = -jumpImpulse;

                localPlayer.jumpStopped = false;
                localPlayer.floorTime = 0.0f;
                localPlayer.jumpTime = 0.0f;
            }
        }
        else {
            // Stop jump on release
            if (!localPlayer.jumpStopped &&
                body.velocity.y < 0.0f &&
                !client->clientInput.jump.pressed) {
                
                body.velocity.y *= jumpStop;
                localPlayer.jumpStopped = true;
            }
        }

        // Fall
        body.velocity.y += gravity * deltaTime;

        // TODO: Make generic position changed flag
        // Update last position, used for packet efficiency
        localPlayer.positionDirty = position.position != localPlayer.lastPosition;
        localPlayer.lastPosition = position.position;

        localPlayer.spriteFlipDirty = spriteFlip.flipX != localPlayer.lastSpriteFlip;
        localPlayer.lastSpriteFlip = spriteFlip.flipX;
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
    maxJumpTime = 0.1f;
}