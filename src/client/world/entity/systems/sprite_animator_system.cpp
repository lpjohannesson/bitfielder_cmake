#include "sprite_animator_system.h"
#include <glm/gtx/common.hpp>
#include "world/entity/systems/entity_system_impl.h"
#include "core/game_time.h"

using namespace bf;

void SpriteAnimatorSystem::playAnimation(SpriteAnimatorComponent &animator, SpriteAnimation &animation) {
    // Skip if already playing
    if (animator.animation == &animation) {
        return;
    }

    animator.animation = &animation;
    animator.time = 0.0f;
}

void SpriteAnimatorSystem::update(World &world) {
    float deltaTime = gameTime.getDeltaTime();

    auto view = world.entities.registry.view<SpriteAnimatorComponent, SpriteComponent>();

    for (auto [entity, spriteAnimator, sprite] : view.each()) {
        if (spriteAnimator.animation == nullptr) {
            continue;
        }

        // Set frame
        sprite.uvBox = spriteAnimator.animation->getFrame(spriteAnimator.time);

        // Advance time
        float duration = spriteAnimator.animation->duration;

        if (duration != 0.0f) {
            spriteAnimator.time = glm::fmod(spriteAnimator.time + deltaTime, duration);
        }
    }
}