#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "client_entity_system.h"
#include "gfx/sprite/sprite_frames.h"

namespace bf {
    struct ParticleSpawnProperties {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec2 size;
        Box2 frame;
        glm::vec4 color;
    };

    struct ParticleExplosionProperties {
        glm::vec2 position;
        glm::vec2 size;
        SpriteFrames &frames;
        glm::vec4 color;
    };

    class ParticleSystem : public ClientEntitySystem {
    public:
        float gravity, explosionSpeedMin, explosionSpeedMax;
        int explosionCountMin, explosionCountMax;

        entt::entity spawnParticle(const ParticleSpawnProperties &properties, WorldScene &scene);
        void spawnParticleExplosion(const ParticleExplosionProperties &properties, WorldScene &scene);

        void update(WorldScene &scene) override;

        ParticleSystem();
    };
}