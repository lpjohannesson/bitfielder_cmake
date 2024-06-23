#include "effect_sprite_system.h"
#include "world/entity/systems/entity_system_impl.h"
#include "../components/effect_sprite_component.h"
#include "../components/sprite_component.h"
#include "../components/sprite_animator_component.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/sprite_animation_component.h"

using namespace bf;

void EffectSpriteProperties::createProperties(glm::vec2 size, Box2 uvBox, glm::ivec2 frameCounts, const std::vector<int> frames, float duration) {
    this->size = size;

    spriteFrames.loadFrames(uvBox, frameCounts);
    animationSet.addAnimation(frames, duration, false);
}

entt::entity EffectSpriteSystem::spawnEffect(World &world, glm::vec2 position, EffectSpriteProperties &properties) {
    entt::registry &entityRegistry = world.entities.registry;

    entt::entity effect = entityRegistry.create();

    entityRegistry.emplace<EffectSpriteComponent>(effect, EffectSpriteComponent {});
    entityRegistry.emplace<PositionComponent>(effect, PositionComponent { position }); 
    entityRegistry.emplace<SpriteComponent>(effect, SpriteComponent { properties.size, properties.size * -0.5f });
    entityRegistry.emplace<SpriteAnimationComponent>(effect, SpriteAnimationComponent { 0 });
    entityRegistry.emplace<SpriteAnimatorComponent>(effect, SpriteAnimatorComponent { &properties.spriteFrames, &properties.animationSet });

    return effect;
}

void EffectSpriteSystem::update(World &world) {
    auto view = world.entities.registry.view<EffectSpriteComponent, SpriteAnimationComponent, SpriteAnimatorComponent>();
    
    for (auto [entity, spriteAnimation, spriteAnimator] : view.each()) {
        // Delete if animation finished
        const SpriteAnimation *animation = spriteAnimator.animationSet->getAnimation(spriteAnimation.animationIndex);

        if (spriteAnimator.time >= animation->duration) {
            world.entities.registry.destroy(entity);
        }
    }
}