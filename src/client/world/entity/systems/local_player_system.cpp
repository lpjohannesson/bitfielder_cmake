#include "local_player_system.h"
#include <glm/gtx/easing.hpp>
#include "client_entity_system_impl.h"
#include "client/scenes/world_scene.h"
#include "core/game_time.h"
#include "core/direction.h"
#include "sprite_aim_system.h"
#include <iostream>

using namespace bf;

void LocalPlayerSystem::move(LocalPlayerData &playerData) {
    float deltaTime = gameTime.getDeltaTime();

    glm::vec2 &movement = playerData.movement;
    LocalPlayerComponent &localPlayer = *playerData.localPlayer;
    PositionComponent &position = *playerData.position;
    VelocityComponent &velocity = *playerData.velocity;
    BodyComponent &body = *playerData.body;

    // Move horizontally, friction only on the ground
    if (body.isOnFloor || movement.x != 0.0f) {
        Direction::targetAxis(velocity.velocity.x, movement.x * speed, acceleration * deltaTime);
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
            velocity.velocity.y = velocity.oldVelocity.y = -jumpImpulse;

            localPlayer.jumpStopped = false;
            localPlayer.floorTime = 0.0f;
            localPlayer.jumpTime = 0.0f;
        }
    }
    else {
        // Stop jump on release
        if (!localPlayer.jumpStopped &&
            velocity.velocity.y < 0.0f &&
            !client->clientInput.jump.pressed) {
            
            velocity.velocity.y *= jumpStop;
            localPlayer.jumpStopped = true;
        }
    }

    // Fall
    velocity.velocity.y += gravity * deltaTime;
}

void LocalPlayerSystem::selectItems(LocalPlayerData &playerData) {
    LocalPlayerComponent &localPlayer = *playerData.localPlayer;

    int selectDirection = 
        (int)client->clientInput.selectItemRight.justPressed() -
        (int)client->clientInput.selectItemLeft.justPressed();
    
    if (selectDirection == 0) {
        return;
    }

    // Select item with wrap-around
    int nextBlockIndex = localPlayer.selectedBlockIndex + selectDirection;
    int blockCount = scene->world.blocks.blocks.size();

    if (nextBlockIndex < 1) {
        nextBlockIndex = blockCount - 1;
    }
    else if (nextBlockIndex >= blockCount) {
        nextBlockIndex = 1;
    }

    localPlayer.selectedBlockIndex = nextBlockIndex;
}

void LocalPlayerSystem::applyAim(LocalPlayerData &playerData) {
    glm::vec2 &movement = playerData.movement;
    LocalPlayerComponent &localPlayer = *playerData.localPlayer;
    SpriteFlipComponent &spriteFlip = *playerData.spriteFlip;
    SpriteAnimatorComponent &spriteAnimator = *playerData.spriteAnimator;
    AimComponent &aim = *playerData.aim;
    SpriteAimComponent &spriteAim = *playerData.spriteAim;

    if (localPlayer.blockTime > 0.0f) {
        return;
    }

    // Flip sprite
    if (movement.x != 0.0f) {
        bool nextSpriteFlip = movement.x < 0.0f;

        if (nextSpriteFlip != spriteFlip.flipX) {
            spriteFlip.flipX = nextSpriteFlip;
            
            playerData.stateChanged = true;
        }
    }

    // Aim vertically
    int nextAim = movement.y;

    if (nextAim != aim.aim) {
        playerData.stateChanged = true;

        aim.aim = nextAim;
        spriteAnimator.frames = SpriteAimSystem::getAimFrames(spriteAim, aim.aim);
        
        playerData.stateChanged = true;
    }
}

void LocalPlayerSystem::animate(LocalPlayerData &playerData) {
    glm::vec2 &movement = playerData.movement;
    LocalPlayerComponent &localPlayer = *playerData.localPlayer;
    VelocityComponent &velocity = *playerData.velocity;
    BodyComponent &body = *playerData.body;
    
    if (localPlayer.blockTime > 0.0f) {
        return;
    }

    PlayerAnimation animationIndex;

    if (body.isOnFloor) {
        if (velocity.velocity.x == 0.0f || movement.x == 0.0f) {
            animationIndex = PlayerAnimation::IDLE;
        }
        else {
            if (glm::sign(velocity.velocity.x) == movement.x) {
                animationIndex = PlayerAnimation::WALK;
            }
            else {
                animationIndex = PlayerAnimation::SLIDE;
            }
        }
    }
    else {
        if (velocity.velocity.y < 0.0f) {
            animationIndex = PlayerAnimation::JUMP;
        }
        else {
            animationIndex = PlayerAnimation::FALL;
        }
    }

    playerData.stateChanged |= SpriteAnimatorSystem::playAnimation(
        *playerData.spriteAnimator, *playerData.spriteAnimation, (int)animationIndex);
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
    VelocityComponent &velocity = *playerData.velocity;
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
    BlockData *blockData;
    bool placing;

    if (onFrontLayer) {
        // Determine if placing, breaking, or neither
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
            blockPosition = &centerBlockPosition;
            blockData = centerBlockData;
        }
        else {
            blockPosition = &forwardBlockPosition;
            blockData = forwardBlockData;
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

        placing = centerBlockData->backIndex == 0;

        blockPosition = &centerBlockPosition;
        blockData = centerBlockData;
    }

    // Replace block
    PlayerAnimation animationIndex;

    if (placing) {
        scene->placeBlock(*blockPosition, onFrontLayer, blockData, localPlayer.selectedBlockIndex);
    }
    else {
        scene->destroyBlock(*blockPosition, onFrontLayer, blockData);
    }

    SpriteAnimatorSystem::playAnimation(*playerData.spriteAnimator, *playerData.spriteAnimation, (int)PlayerAnimation::PUNCH);

    // Send to server
    scene->writeReplaceBlock(*blockPosition, onFrontLayer, blockData);
    
    // TODO: Disable body
    velocity.velocity = { 0.0f, 0.0f };

    // Reset timers
    localPlayer.blockTime = maxBlockTime;
    localPlayer.blockTweenTime = maxBlockTweenTime;

    // Set up movement animation
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
        VelocityComponent,
        BodyComponent,
        SpriteComponent,
        SpriteFlipComponent,
        SpriteAnimationComponent,
        SpriteAnimatorComponent,
        AimComponent,
        SpriteAimComponent>();

    for (auto [
            entity,
            localPlayer,
            position,
            velocity,
            body,
            sprite,
            spriteFlip,
            spriteAnimation,
            spriteAnimator,
            aim,
            spriteAim] : view.each()) {
        
        LocalPlayerData playerData = {
            movement,
            &localPlayer,
            &position,
            &velocity,
            &body,
            &sprite,
            &spriteFlip,
            &spriteAnimation,
            &spriteAnimator,
            &aim,
            &spriteAim };

        applyAim(playerData);
        animate(playerData);

        selectItems(playerData);

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

        // TODO: Make generic position changed flag
        if (position.position != localPlayer.lastPosition) {
            playerData.stateChanged = true;
        }

        localPlayer.lastPosition = position.position;

        if (playerData.stateChanged) {
            scene->writePlayerState();
        }
    }
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