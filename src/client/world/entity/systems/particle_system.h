#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "client_entity_system.h"
#include "gfx/sprite/animation/sprite_frames.h"

namespace bf {
    class ParticleSystem : public ClientEntitySystem {
    public:
        float gravity, explosionSpeedMin, explosionSpeedMax;
        int explosionCountMin, explosionCountMax;

        entt::entity spawnParticle(glm::vec2 position, glm::vec2 velocity, glm::vec2 size, Box2 frame, glm::vec4 color);
        void spawnParticleExplosion(glm::vec2 position, glm::vec2 size, const SpriteFrames &frames, glm::vec4 color);

        void update(World &world) override;

        ParticleSystem();
    };
}