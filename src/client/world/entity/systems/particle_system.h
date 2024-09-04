#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "client_entity_system.h"
#include "gfx/sprite/sprite_set.h"

namespace bf {
    struct ParticleSpawnProperties {
        Box2 uvBox;
        glm::vec2 position{}, velocity{}, size{};
        glm::vec4 color{ 1.0f };
    };

    struct ParticleExplosionProperties {
        SpriteSet &sprites;
        glm::vec2 position{}, size{};
        glm::vec4 color{ 1.0f };
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