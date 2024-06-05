#pragma once
#include <random>
#include <glm/glm.hpp>
#include "client_entity_system.h"
#include "gfx/sprite/animation/sprite_frames.h"

namespace bf {
    class ParticleSystem : public ClientEntitySystem {
    private:
        std::default_random_engine randomEngine;
        std::uniform_int_distribution<int> randomInt;
        std::uniform_real_distribution<float> randomFloat;

    public:
        float gravity, explosionSpeedMin, explosionSpeedMax;
        int explosionCountMin, explosionCountMax;

        void spawnParticle(glm::vec2 position, glm::vec2 velocity, glm::vec2 size, Box2 frame);
        void spawnParticleExplosion(glm::vec2 position, glm::vec2 size, const SpriteFrames &frames);

        void update(World &world) override;

        ParticleSystem();
    };
}