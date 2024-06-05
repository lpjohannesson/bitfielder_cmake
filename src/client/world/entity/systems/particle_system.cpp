#include "particle_system.h"
#include "client_entity_system_impl.h"
#include "../components/particle_component.h"
#include "../components/sprite_component.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/velocity_component.h"
#include "core/game_time.h"

using namespace bf;

void ParticleSystem::spawnParticle(glm::vec2 position, glm::vec2 velocity, glm::vec2 size, Box2 frame) {
    entt::registry &entityRegistry = scene->world.entities.registry;

    // Create entity directly, without ID
    entt::entity particle = entityRegistry.create();

    entityRegistry.emplace<ParticleComponent>(particle, ParticleComponent {});
    entityRegistry.emplace<PositionComponent>(particle, PositionComponent { position });
    entityRegistry.emplace<VelocityComponent>(particle, VelocityComponent { velocity });
    entityRegistry.emplace<SpriteComponent>(particle, SpriteComponent { size, -size * 0.5f, frame });
}

void ParticleSystem::spawnParticleExplosion(glm::vec2 position, glm::vec2 size, const SpriteFrames &frames) {
    // Random count
    int explosionCountRange = explosionCountMax - explosionCountMin + 1;
    int explosionCount = explosionCountMin + randomInt(randomEngine) % explosionCountRange;

    for (int i = 0; i < explosionCount; i++) {
        // Random speed
        float explosionSpeedRange = explosionSpeedMax - explosionSpeedMin;
        float explosionSpeed = explosionSpeedMin + randomFloat(randomEngine) * explosionSpeedRange;

        // Random angle
        float angle = glm::radians((float)(rand() % 360));
        glm::vec2 velocity = { glm::cos(angle), glm::sin(angle) };

        // Random frame
        Box2 frame = frames.frames.at(rand() % frames.frames.size());

        spawnParticle(position, velocity * explosionSpeed, size, frame);
    }
}

void ParticleSystem::update(World &world) {
    float deltaTime = gameTime.getDeltaTime();
    Box2 screenBox = scene->worldRenderer.getScreenBox();

    auto view = world.entities.registry.view<ParticleComponent, PositionComponent, VelocityComponent, SpriteComponent>();

    for (auto [entity, position, velocity, sprite] : view.each()) {
        velocity.velocity.y += gravity * deltaTime;
        position.position += velocity.velocity * deltaTime;

        // Destroy if particle is offscreen
        Box2 box = { position.position + sprite.offset, sprite.size };

        if (!Box2::overlaps(box, screenBox)) {
            world.entities.registry.destroy(entity);
        }
    }
}

ParticleSystem::ParticleSystem() : randomInt(0, INT_MAX), randomFloat(0.0f, 1.0f) {
    gravity = 20.0f;
    explosionSpeedMin = 4.0f;
    explosionSpeedMax = 7.0f;
    explosionCountMin = 4;
    explosionCountMax = 7;
}