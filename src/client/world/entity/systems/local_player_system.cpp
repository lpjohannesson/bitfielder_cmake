#include "local_player_system.h"
#include "client/scenes/world_scene.h"
#include <iostream>
#include <glm/gtx/easing.hpp>
#include "client/scenes/world_scene.h"
#include "core/game_time.h"
#include "core/direction.h"
#include "sprite_aim_system.h"
#include "sound/sound.h"
#include "world/item/components/item_block_component.h"

using namespace bf;

void LocalPlayerSystem::move(LocalPlayerData &data) {
    float deltaTime = gameTime.getDeltaTime();

    // Move horizontally, friction only on the ground
    if (data.body.isOnFloor || data.movement.x != 0.0f) {
        Direction::targetAxis(data.velocity.velocity.x, data.movement.x * speed, acceleration * deltaTime);
    }

    // Fall
    data.velocity.velocity.y += gravity * deltaTime;

    // Hit ground
    bool onSurface = data.body.isOnFloor || data.body.isOnCeiling;

    if (onSurface && !data.localPlayer.lastOnSurface) {
        engine->sound.playSound(data.scene.clientContent.groundSound, false);

        if (!data.body.blockCollisions.empty()) {
            for (BlockCollision &blockCollision : data.body.blockCollisions) {
                if (blockCollision.normal.y == 0.0f) {
                    continue;
                }

                entt::entity surfaceBlock = data.scene.world.blocks.getEntity(data.body.blockCollisions[0].blockData->getFrontIndex());
                BlockSounds::playBlockSound(data.scene, surfaceBlock, 0.25f);

                glm::vec2 effectPosition = data.position.position + data.body.size * 0.5f;
                effectPosition.y -= blockCollision.normal.y * data.body.size.y * 0.5f;

                EffectSpriteSystem::spawnEffect(effectPosition,
		            data.scene.clientContent.groundEffectProperties, data.scene);

                break;
            }
        }
    }

    data.localPlayer.lastOnSurface = onSurface;
}

void LocalPlayerSystem::jump(LocalPlayerData &data) {
    // Update jump timer, allowing early jumps
    if (client->clientInput.jump.justPressed()) {
        data.localPlayer.jumpTime = maxJumpTime;
    }

    // Update floor timer, allowing late jumps
    if (data.body.isOnFloor) {
        data.localPlayer.floorTime = maxFloorTime;
    }

    bool canJump = !data.scene.paused && data.localPlayer.floorTime > 0.0f;

    if (canJump) {
        // Jump
        if (data.localPlayer.jumpTime > 0.0f) {
            data.velocity.velocity.y = data.velocity.oldVelocity.y = -jumpImpulse;

            data.localPlayer.jumpStopped = false;
            data.localPlayer.floorTime = 0.0f;
            data.localPlayer.jumpTime = 0.0f;

            engine->sound.playSound(data.scene.clientContent.jumpSound, false, 0.75f, client->getRandomPitch());
        }
    }
    else {
        // Stop jump on release
        bool jumpReleased = data.scene.paused || !client->clientInput.jump.pressed();

        if (!data.localPlayer.jumpStopped &&
            data.velocity.velocity.y < 0.0f &&
            jumpReleased) {
            
            data.velocity.velocity.y *= jumpStop;
            data.localPlayer.jumpStopped = true;
        }
    }
}

void LocalPlayerSystem::selectItems(LocalPlayerData &data) {
    if (data.scene.paused) {
        return;
    }

    int selectDirection = 
        (int)client->clientInput.selectItemRight.justPressed() -
        (int)client->clientInput.selectItemLeft.justPressed();
    
    if (selectDirection == 0) {
        return;
    }

    // Select item with wrap-around
    Inventory &inventory = data.inventory.inventory;

    int nextBlockIndex = inventory.selectedIndex + selectDirection;

    inventory.selectedIndex =
        Direction::posmod(nextBlockIndex, (int)inventory.items.size());

    data.scene.worldRenderer.hud.updateMesh(data.scene);

    engine->sound.playSound(data.scene.clientContent.selectItemSound, false, 0.75f, client->getRandomPitch());
}

void LocalPlayerSystem::applyAim(LocalPlayerData &data) {
    if (data.scene.paused) {
        return;
    }

    if (data.localPlayer.blockTime > 0.0f) {
        return;
    }

    // Flip sprite
    if (data.movement.x != 0.0f) {
        bool nextSpriteFlip = data.movement.x < 0.0f;

        if (nextSpriteFlip != data.spriteFlip.flipX) {
            data.spriteFlip.flipX = nextSpriteFlip;
            
            data.stateChanged = true;
        }
    }

    // Aim vertically
    int nextAim = (int)data.movement.y;

    if (nextAim != data.aim.aim) {
        data.stateChanged = true;

        data.aim.aim = nextAim;
        data.spriteAnimator.frames = SpriteAimSystem::getAimFrames(data.spriteAim, data.aim.aim);
        
        data.stateChanged = true;
    }
}

void LocalPlayerSystem::animate(LocalPlayerData &data) {
    if (data.localPlayer.blockTime > 0.0f) {
        return;
    }

    PlayerAnimation animationIndex;

    if (data.body.isOnFloor) {
        if (data.velocity.velocity.x == 0.0f || data.movement.x == 0.0f) {
            animationIndex = PlayerAnimation::IDLE;
        }
        else {
            if (glm::sign(data.velocity.velocity.x) == data.movement.x) {
                animationIndex = PlayerAnimation::WALK;
            }
            else {
                animationIndex = PlayerAnimation::SLIDE;
            }
        }
    }
    else {
        if (data.velocity.velocity.y < 0.0f) {
            animationIndex = PlayerAnimation::JUMP;
        }
        else {
            animationIndex = PlayerAnimation::FALL;
        }
    }

    data.stateChanged |= SpriteAnimatorSystem::playAnimation(
        data.spriteAnimator, data.spriteAnimation, (int)animationIndex);
}

bool LocalPlayerSystem::tryModifyBlock(LocalPlayerData &data) {
    if (data.scene.paused) {
        return false;
    }

    // Check input
    bool onFrontLayer;

    if (client->clientInput.modifyBlockFront.pressed()) {
        onFrontLayer = true;
    }
    else if (client->clientInput.modifyBlockBack.pressed()) {
        onFrontLayer = false;
    }
    else {
        return false;
    }

    float deltaTime = gameTime.getDeltaTime();

    // Get center and forward blocks
    glm::vec2 playerCenter = data.position.position + data.body.size * 0.5f;
    glm::ivec2 centerBlockPosition = glm::floor(playerCenter);

    glm::ivec2 forwardBlockPosition;

    if (data.movement.y == 0.0f) {
        int facingSign = data.spriteFlip.flipX ? -1 : 1;
        forwardBlockPosition = { centerBlockPosition.x + facingSign, centerBlockPosition.y };
    }
    else {
        forwardBlockPosition = { centerBlockPosition.x, centerBlockPosition.y + (int)data.movement.y };
    }

    BlockData *centerBlockData = BlockChunk::getWorldBlock(data.scene.world.map, centerBlockPosition);
    BlockData *forwardBlockData = BlockChunk::getWorldBlock(data.scene.world.map, forwardBlockPosition);

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
        if (!data.scene.world.isBlockPlaceable(*blockPosition, onFrontLayer)) {
            return false;
        }

        // Get block from inventory
        Inventory &inventory = data.inventory.inventory;
        entt::entity selectedItem;
        
        if (!inventory.getSelectedItem(data.scene.world, selectedItem)) {
            return false;
        }

        entt::registry &itemsRegistry = data.scene.world.items.registry;

        if (!itemsRegistry.all_of<ItemBlockComponent>(selectedItem)) {
            return false;
        }

        int blockIndex = itemsRegistry.get<ItemBlockComponent>(selectedItem).blockIndex;
        data.scene.placeBlock(blockIndex, *blockPosition, onFrontLayer, *blockData);
    }
    else {
        data.scene.destroyBlock(*blockPosition, onFrontLayer, *blockData);
    }

    // Play animation
    SpriteAnimatorSystem::playAnimation(data.spriteAnimator, data.spriteAnimation, (int)PlayerAnimation::PUNCH);

    // Send to server
    ClientPacketManager::writeReplaceBlock(*blockPosition, onFrontLayer, *blockData, data.scene);
    
    // TODO: Disable body
    data.velocity.velocity = { 0.0f, 0.0f };

    // Reset timers
    data.localPlayer.blockTime = maxBlockTime;
    data.localPlayer.blockTweenTime = maxBlockTweenTime;

    // Set up movement animation
    glm::vec2 positionOffset = glm::vec2(0.5f) - data.body.size * 0.5f;
    glm::vec2 endPosition = glm::vec2(forwardBlockPosition) + positionOffset;

    data.localPlayer.blockTweenStart = data.position.position;
    data.localPlayer.blockTweenEnd = endPosition;

    return true;
}

void LocalPlayerSystem::update(WorldScene &scene) {
    float deltaTime = gameTime.getDeltaTime();

    // Input direction
    glm::vec2 movement;

    if (scene.paused) {
        movement = { 0.0f, 0.0f };
    }
    else {
        movement = {
        (float)client->clientInput.right.pressed() - (float)client->clientInput.left.pressed(),
        (float)client->clientInput.down.pressed() - (float)client->clientInput.up.pressed()
    };
    }

    auto view = scene.world.entities.registry.view<
        LocalPlayerComponent,
        PositionComponent,
        VelocityComponent,
        BodyComponent,
        SpriteComponent,
        SpriteFlipComponent,
        SpriteAnimationComponent,
        SpriteAnimatorComponent,
        AimComponent,
        SpriteAimComponent,
        InventoryComponent>();

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
            spriteAim,
            inventory] : view.each()) {
        
        LocalPlayerData data = {
            scene,
            movement,
            localPlayer,
            position,
            velocity,
            body,
            sprite,
            spriteFlip,
            spriteAnimation,
            spriteAnimator,
            aim,
            spriteAim,
            inventory };

        applyAim(data);
        animate(data);

        selectItems(data);

        if (localPlayer.blockTime == 0.0f) {
            if (!tryModifyBlock(data)) {
                jump(data);
                move(data);
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
            data.stateChanged = true;
        }

        localPlayer.lastPosition = position.position;

        if (data.stateChanged) {
            ClientPacketManager::writePlayerState(scene);
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