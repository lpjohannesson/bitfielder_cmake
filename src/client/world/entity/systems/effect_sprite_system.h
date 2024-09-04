#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "core/box2.h"
#include "gfx/sprite/sprite_set.h"
#include "client_entity_system.h"
#include "../animation/animation_set.h"

namespace bf {
    class EffectSpriteProperties {
    public:
        SpriteSet sprites;
        AnimationSet animations;
        glm::vec2 size;

        void createProperties(Box2 box, glm::ivec2 counts, const std::vector<int> frames, float duration, glm::vec2 size);
    };

    class EffectSpriteSystem : public ClientEntitySystem {
    public:
        static entt::entity spawnEffect(glm::vec2 position, EffectSpriteProperties &properties, WorldScene &scene);

        void update(WorldScene &scene) override;
    };
}