#include "effect_sprite_system.h"
#include "client/scenes/world_scene.h"
#include "../components/effect_sprite_component.h"
#include "../components/sprite_component.h"
#include "../components/sprite_set_component.h"
#include "../components/animator_component.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/animation_component.h"

using namespace bf;

void EffectSpriteProperties::createProperties(Box2 box, glm::ivec2 counts, const std::vector<int> frames, float duration, glm::vec2 size) {
    sprites.loadSprites(box, counts);
    animations.addAnimation(frames, duration, false);
    this->size = size;
}

entt::entity EffectSpriteSystem::spawnEffect(glm::vec2 position, EffectSpriteProperties &properties, WorldScene &scene) {
    entt::registry &entityRegistry = scene.world.entities.registry;

    entt::entity effect = entityRegistry.create();

    entityRegistry.emplace<EffectSpriteComponent>(effect, EffectSpriteComponent {});
    entityRegistry.emplace<PositionComponent>(effect, PositionComponent { position }); 
    entityRegistry.emplace<SpriteComponent>(effect, SpriteComponent { properties.size, properties.size * -0.5f });
    entityRegistry.emplace<SpriteSetComponent>(effect, SpriteSetComponent { &properties.sprites });
    entityRegistry.emplace<AnimationComponent>(effect, AnimationComponent {});
    entityRegistry.emplace<AnimatorComponent>(effect, AnimatorComponent { &properties.animations });

    return effect;
}

void EffectSpriteSystem::update(WorldScene &scene) {
    auto view = scene.world.entities.registry.view<EffectSpriteComponent, AnimationComponent, AnimatorComponent>();
    
    for (auto [entity, animation, animator] : view.each()) {
        // Delete if animation finished
        const Animation *currentAnimation = animator.animations->getAnimation(animation.index);

        if (animator.time >= currentAnimation->duration) {
            scene.world.entities.registry.destroy(entity);
        }
    }
}