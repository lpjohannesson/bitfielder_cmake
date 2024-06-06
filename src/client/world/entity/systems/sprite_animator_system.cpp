#include "sprite_animator_system.h"
#include <glm/gtx/common.hpp>
#include "world/entity/systems/entity_system_impl.h"
#include "../components/sprite_component.h"
#include "core/game_time.h"

using namespace bf;

bool SpriteAnimatorSystem::playAnimation(SpriteAnimatorComponent &spriteAnimator, SpriteAnimationComponent &spriteAnimation, int animationIndex) {
    const SpriteAnimation *animation = spriteAnimator.animationSet->getAnimation(animationIndex);

    if (spriteAnimation.animationIndex == animationIndex) {
        if (animation->loops) {
            // Skip if already playing
            return false;
        }
        else {
            // Skip if not ended
            if (spriteAnimator.time < animation->duration) {
                return false;
            }
        }
    }

    spriteAnimation.animationIndex = animationIndex;
    spriteAnimator.time = 0.0f;

    return true;
}

void SpriteAnimatorSystem::update(World &world) {
    float deltaTime = gameTime.getDeltaTime();

    auto view = world.entities.registry.view<SpriteAnimatorComponent, SpriteComponent, SpriteAnimationComponent>();

    for (auto [entity, spriteAnimator, sprite, spriteAnimation] : view.each()) {
        // Get animation or skip
        SpriteFrames *frames = spriteAnimator.frames;

        if (frames == nullptr) {
            continue;
        }

        SpriteAnimationSet *animationSet = spriteAnimator.animationSet;

        if (animationSet == nullptr) {
            continue;
        }

        SpriteAnimation *animation = animationSet->getAnimation(spriteAnimation.animationIndex);

        if (animation == nullptr) {
            continue;
        }

        // Set frame
        animationSet->getFrame(*frames, *animation, spriteAnimator.time, sprite.uvBox);

        // Advance time
        float duration = animation->duration;

        if (duration != 0.0f) {
            if (animation->loops) {
                spriteAnimator.time = glm::fmod(spriteAnimator.time + deltaTime, duration);
            }
            else {
                spriteAnimator.time = glm::min(spriteAnimator.time + deltaTime, duration);
            }
        }
    }
}