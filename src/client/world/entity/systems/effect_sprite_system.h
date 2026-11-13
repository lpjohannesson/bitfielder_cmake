#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "client_entity_system.h"
#include "gfx/sprite/animation/sprite_animation_set.h"

namespace bf {
    class EffectSpriteProperties {
    public:
        glm::vec2 size;
        SpriteFrames spriteFrames;
        SpriteAnimationSet animationSet;

        void createProperties(glm::vec2 size, Box2 uvBox, glm::ivec2 frameCounts, const std::vector<int> frames, float duration);
    };

    class EffectSpriteSystem : public ClientEntitySystem {
    public:
        static entt::entity spawnEffect(glm::vec2 position, EffectSpriteProperties &properties, WorldScene &scene);

        void update(WorldScene &scene) override;
    };
}