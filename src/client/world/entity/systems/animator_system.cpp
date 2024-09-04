#include "animator_system.h"
#include "client/scenes/world_scene.h"
#include <glm/gtx/common.hpp>
#include "core/game_time.h"

using namespace bf;

bool AnimatorSystem::playAnimation(AnimatorComponent &animator, AnimationComponent &animation, int index) {
    if (animation.index == index) {
        Animation *currentAnimation = animator.animations->getAnimation(index);

        if (currentAnimation->loops) {
            // Skip if already playing
            return false;
        }
        else {
            // Skip if not ended
            if (animator.time < currentAnimation->duration) {
                return false;
            }
        }
    }

    animation.index = index;
    animator.time = 0.0f;

    return true;
}

void AnimatorSystem::update(WorldScene &scene) {
    float deltaTime = gameTime.getDeltaTime();

    auto view = scene.world.entities.registry.view<AnimatorComponent, AnimationComponent>();

    for (auto [entity, animator, animation] : view.each()) {
        Animation *currentAnimation = animator.animations->getAnimation(animation.index);
        float duration = currentAnimation->duration;

        if (duration != 0.0f) {
            if (currentAnimation->loops) {
                animator.time = glm::fmod(animator.time + deltaTime, duration);
            }
            else {
                animator.time = glm::min(animator.time + deltaTime, duration);
            }
        }
    }
}