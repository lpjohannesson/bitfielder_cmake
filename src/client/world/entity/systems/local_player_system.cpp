#include "local_player_system.h"
#include <iostream>
#include <glm/gtx/easing.hpp>
#include "client_entity_system_impl.h"
#include "client/scenes/world_scene.h"
#include "core/game_time.h"
#include "core/direction.h"
#include "sprite_aim_system.h"
#include "sound/sound.h"

using namespace bf;

void LocalPlayerSystem::move(LocalPlayerData &playerData) {
    float deltaTime = gameTime.getDeltaTime();

    LocalPlayerComponent &localPlayer = *playerData.localPlayer;
    glm::vec2 &movement = playerData.movement;
    PositionComponent &position = *playerData.position;
    VelocityComponent &velocity = *playerData.velocity;
    BodyComponent &body = *playerData.body;

    // Move horizontally, friction only on the ground
    if (body.isOnFloor || movement.x != 0.0f) {
        Direction::targetAxis(velocity.velocity.x, movement.x * speed, acceleration * deltaTime);
    }

    // Fall
    velocity.velocity.y += gravity * deltaTime;

    // Hit ground
    bool onSurface = body.isOnFloor || body.isOnCeiling;

    if (onSurface && !localPlayer.lastOnSurface) {
        engine->sound.playSound(scene->clientContent.groundSound, false);

        if (!body.blockCollisions.empty()) {
            for (BlockCollision &blockCollision : body.blockCollisions) {
                if (blockCollision.normal.y == 0.0f) {
                    continue;
                }

                entt::entity surfaceBlock = scene->world.blocks.getBlock(body.blockCollisions[0].blockData->getFrontIndex());
                BlockSounds::playBlockSound(*scene, surfaceBlock, 0.25f);

                glm::vec2 effectPosition = position.position + body.size * 0.5f;
                effectPosition.y -= blockCollision.normal.y * body.size.y * 0.5f;

                EffectSpriteSystem::spawnEffect(scene->world, effectPosition,
		            scene->clientContent.groundEffectProperties);

                break;
            }
        }
    }

    localPlayer.lastOnSurface = onSurface;
}

void LocalPlayerSystem::jump(LocalPlayerData &playerData) {
    LocalPlayerComponent &localPlayer = *playerData.localPlayer;
    VelocityComponent &velocity = *playerData.velocity;
    BodyComponent &body = *playerData.body;

    // Update jump timer, allowing early jumps
    if (client->clientInput.jump.justPressed()) {
        localPlayer.jumpTime = maxJumpTime;
    }

    // Update floor timer, allowing late jumps
    if (body.isOnFloor) {
        localPlayer.floorTime = maxFloorTime;
    }

    bool canJump = !scene->paused && localPlayer.floorTime > 0.0f;

    if (canJump) {
        // Jump
        if (localPlayer.jumpTime > 0.0f) {
            velocity.velocity.y = velocity.oldVelocity.y = -jumpImpulse;

            localPlayer.jumpStopped = false;
            localPlayer.floorTime = 0.0f;
            localPlayer.jumpTime = 0.0f;

            engine->sound.playSound(scene->clientContent.jumpSound, false, 0.75f, client->getRandomPitch());
        }
    }
    else {
        // Stop jump on release
        bool jumpReleased = scene->paused || !client->clientInput.jump.pressed;

        if (!localPlayer.jumpStopped &&
            velocity.velocity.y < 0.0f &&
            jumpReleased) {
            
            velocity.velocity.y *= jumpStop;
            localPlayer.jumpStopped = true;
        }
    }
}

void LocalPlayerSystem::selectItems(LocalPlayerData &playerData) {
    if (scene->paused) {
        return;
    }

    LocalPlayerComponent &localPlayer = *playerData.localPlayer;

    int selectDirection = 
        (int)client->clientInput.selectItemRight.justPressed() -
        (int)client->clientInput.selectItemLeft.justPressed();
    
    if (selectDirection == 0) {
        return;
    }

    // Select item with wrap-around
    int nextBlockIndex = localPlayer.selectedBlockIndex + selectDirection;
    localPlayer.selectedBlockIndex = Direction::posmod(nextBlockIndex, (int)scene->world.blocks.blocks.size());
}

void LocalPlayerSystem::applyAim(LocalPlayerData &playerData) {
    if (scene->paused) {
        return;
    }

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
    int nextAim = (int)movement.y;

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
    if (scene->paused) {
        return false;
    }

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

            if (forwardBlockData->getFrontIndex() == 0) {
                return false;
            }

            placing = false;
        }
        else {
            placing = forwardBlockData->getFrontIndex() == 0;
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
            if (forwardBlockData->getFrontIndex() != 0) {
                return false;
            }
        }

        placing = centerBlockData->getBackIndex() == 0;

        blockPosition = &centerBlockPosition;
        blockData = centerBlockData;
    }

    // Replace block
    if (placing) {
        if (!scene->world.isBlockPlaceable(*blockPosition, onFrontLayer)) {
            return false;
        }

        scene->placeBlock(*blockPosition, onFrontLayer, *blockData, localPlayer.selectedBlockIndex);
    }
    else {
        scene->destroyBlock(*blockPosition, onFrontLayer, *blockData);
    }

    // Play animation
    SpriteAnimatorSystem::playAnimation(*playerData.spriteAnimator, *playerData.spriteAnimation, (int)PlayerAnimation::PUNCH);

    // Send to server
    scene->writeReplaceBlock(*blockPosition, onFrontLayer, *blockData);
    
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
    glm::vec2 movement;

    if (scene->paused) {
        movement = { 0.0f, 0.0f };
    }
    else {
        movement = {
        (float)client->clientInput.right.pressed - (float)client->clientInput.left.pressed,
        (float)client->clientInput.down.pressed - (float)client->clientInput.up.pressed
    };
    }

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
                jump(playerData);
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