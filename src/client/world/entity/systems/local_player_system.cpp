#include "local_player_system.h"
#include <glm/gtx/easing.hpp>
#include "world/entity/systems/entity_system_impl.h"
#include "client/scenes/world_scene.h"
#include "core/game_time.h"
#include "core/direction.h"
#include <iostream>

using namespace bf;

void LocalPlayerSystem::move(LocalPlayerData &playerData) {
    float deltaTime = gameTime.getDeltaTime();

    glm::vec2 &movement = playerData.movement;
    LocalPlayerComponent &localPlayer = *playerData.localPlayer;
    PositionComponent &position = *playerData.position;
    BodyComponent &body = *playerData.body;

    // Move horizontally, friction only on the ground
    if (body.isOnFloor || movement.x != 0.0f) {
        Direction::targetAxis(body.velocity.x, movement.x * speed, acceleration * deltaTime);
    }

    // Update jump timer, allowing early jumps
    if (client->clientInput.jump.justPressed()) {
        localPlayer.jumpTime = maxJumpTime;
    }

    // Update floor timer, allowing late jumps
    if (body.isOnFloor) {
        localPlayer.floorTime = maxFloorTime;
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
}

void LocalPlayerSystem::animate(LocalPlayerData &playerData) {
    // TODO: Sprite aim

    glm::vec2 &movement = playerData.movement;
    LocalPlayerComponent &localPlayer = *playerData.localPlayer;
    BodyComponent &body = *playerData.body;
    SpriteFlipComponent &spriteFlip = *playerData.spriteFlip;

    // Flip sprite
    if (movement.x != 0.0f) {
        spriteFlip.flipX = movement.x < 0.0f;
    }

    localPlayer.spriteFlipDirty = spriteFlip.flipX != localPlayer.lastSpriteFlip;
    localPlayer.lastSpriteFlip = spriteFlip.flipX;

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

    localPlayer.spriteAnimationDirty =
        SpriteAnimatorSystem::playAnimation(*playerData.spriteAnimator, *playerData.spriteAnimation, (int)animationIndex);
}

bool LocalPlayerSystem::tryModifyBlock(LocalPlayerData &playerData) {
    // Check input
    bool onFrontLayer;

    if (client->clientInput.modifyBlockFront.pressed) {
        onFrontLayer = true;
    }
    else if (client->clientInput.modifyBlockBack.pressed) {
        onFrontLayer = false;
    }
    else {
        return false;
    }

    float deltaTime = gameTime.getDeltaTime();

    glm::vec2 &movement = playerData.movement;
    LocalPlayerComponent &localPlayer = *playerData.localPlayer;
    PositionComponent &position = *playerData.position;
    BodyComponent &body = *playerData.body;
    SpriteFlipComponent &spriteFlip = *playerData.spriteFlip;

    // Get center and forward blocks
    glm::vec2 playerCenter = position.position + body.size * 0.5f;
    glm::ivec2 centerBlockPosition = glm::floor(playerCenter);

    glm::ivec2 forwardBlockPosition;

    if (movement.y == 0.0f) {
        int facingSign = spriteFlip.flipX ? -1 : 1;
        forwardBlockPosition = { centerBlockPosition.x + facingSign, centerBlockPosition.y };
    }
    else {
        forwardBlockPosition = { centerBlockPosition.x, centerBlockPosition.y + (int)movement.y };
    }

    BlockData *centerBlockData = BlockChunk::getWorldBlock(scene->world.map, centerBlockPosition);
    BlockData *forwardBlockData = BlockChunk::getWorldBlock(scene->world.map, forwardBlockPosition);

    glm::ivec2 *blockPosition;

    if (onFrontLayer) {
        // Determine if placing, breaking, or neither
        bool placing;

        if (forwardBlockData == nullptr) {
            if (centerBlockData == nullptr) {
                return false;
            }

            placing = true;
        }
        else if (centerBlockData == nullptr) {
            if (forwardBlockData == nullptr) {
                return false;
            }

            if (forwardBlockData->frontIndex == 0) {
                return false;
            }

            placing = false;
        }
        else {
            placing = forwardBlockData->frontIndex == 0;
        }

        if (placing) {
            // TODO: Check neighbours and other bodies
            centerBlockData->frontIndex = 1;
            blockPosition = &centerBlockPosition;
        }
        else {
            forwardBlockData->frontIndex = 0;
            blockPosition = &forwardBlockPosition;
        }
    }
    else {
        // Check block exists
        if (centerBlockData == nullptr) {
            return false;
        }

        // Check for block in front
        if (forwardBlockData != nullptr) {
            if (forwardBlockData->frontIndex != 0) {
                return false;
            }
        }

        if (centerBlockData->backIndex == 0) {
            centerBlockData->backIndex = 1;
        }
        else {
            centerBlockData->backIndex = 0;
        }
    
        blockPosition = &centerBlockPosition;
    }

    scene->updateBlock(*blockPosition);
    
    // TODO: Disable body
    body.velocity = { 0.0f, 0.0f };

    // Reset timers
    localPlayer.blockTime = maxBlockTime;
    localPlayer.blockTweenTime = maxBlockTweenTime;

    // Set up movement tween
    glm::vec2 positionOffset = glm::vec2(0.5f) - body.size * 0.5f;
    glm::vec2 endPosition = glm::vec2(forwardBlockPosition) + positionOffset;

    localPlayer.blockTweenStart = position.position;
    localPlayer.blockTweenEnd = endPosition;

    return true;
}

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
        LocalPlayerData playerData = { movement, &localPlayer, &position, &body, &sprite, &spriteFlip, &spriteAnimation, &spriteAnimator };

        animate(playerData);

        if (localPlayer.blockTime == 0.0f) {
            if (!tryModifyBlock(playerData)) {
                move(playerData);
            }
        }
        else {
            localPlayer.blockTime = glm::max(0.0f, localPlayer.blockTime - deltaTime);

            // Apply block tween
            if (localPlayer.blockTweenTime > 0.0f) {
                localPlayer.blockTweenTime = glm::max(0.0f, localPlayer.blockTweenTime - deltaTime);

                float blockTweenProgress = glm::quadraticEaseOut(1.0f - localPlayer.blockTweenTime / maxBlockTweenTime);
                position.position = glm::lerp(localPlayer.blockTweenStart, localPlayer.blockTweenEnd, blockTweenProgress);
            }
        }

        // Update timers
        localPlayer.floorTime = glm::max(0.0f, localPlayer.floorTime - deltaTime);
        localPlayer.jumpTime = glm::max(0.0f, localPlayer.jumpTime - deltaTime);
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
    maxBlockTime = 0.3f;
    maxBlockTweenTime = 0.15f;
}