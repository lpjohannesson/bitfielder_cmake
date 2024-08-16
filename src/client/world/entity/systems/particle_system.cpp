#include "particle_system.h"
#include "client/scenes/world_scene.h"
#include "../components/particle_component.h"
#include "../components/sprite_component.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/velocity_component.h"
#include "core/game_time.h"

using namespace bf;

entt::entity ParticleSystem::spawnParticle(const ParticleSpawnProperties &properties, WorldScene &scene) {
    entt::registry &entityRegistry = scene.world.entities.registry;

    // Create entity directly, without ID
    entt::entity particle = entityRegistry.create();

    entityRegistry.emplace<ParticleComponent>(particle, ParticleComponent {});
    entityRegistry.emplace<PositionComponent>(particle, PositionComponent { properties.position });
    entityRegistry.emplace<VelocityComponent>(particle, VelocityComponent { properties.velocity, properties.velocity });
    entityRegistry.emplace<SpriteComponent>(particle, SpriteComponent { properties.size, -properties.size * 0.5f, properties.color, properties.frame });

    return particle;
}

void ParticleSystem::spawnParticleExplosion(const ParticleExplosionProperties &properties, WorldScene &scene) {
    // Random count
    int explosionCountRange = explosionCountMax - explosionCountMin + 1;
    int explosionCount = explosionCountMin + client->randomInt(client->randomEngine) % explosionCountRange;

    for (int i = 0; i < explosionCount; i++) {
        // Random speed
        float explosionSpeedRange = explosionSpeedMax - explosionSpeedMin;
        float explosionSpeed = explosionSpeedMin + client->randomFloat(client->randomEngine) * explosionSpeedRange;

        // Random angle
        float angle = glm::radians((float)(rand() % 360));
        glm::vec2 velocity = { glm::cos(angle), glm::sin(angle) };

        // Random frame
        Box2 frame = properties.frames.frames.at(rand() % properties.frames.frames.size());

        spawnParticle({ frame, properties.position, velocity * explosionSpeed, properties.size, properties.color }, scene);
    }
}

void ParticleSystem::update(WorldScene &scene) {
    float deltaTime = gameTime.getDeltaTime();
    Box2 screenBox = scene.worldRenderer.getScreenBox();

    auto view = scene.world.entities.registry.view<ParticleComponent, PositionComponent, VelocityComponent, SpriteComponent>();

    for (auto [entity, position, velocity, sprite] : view.each()) {
        velocity.velocity.y += gravity * deltaTime;
        
        position.position += (velocity.velocity + velocity.oldVelocity) * 0.5f * deltaTime;
        velocity.oldVelocity = velocity.velocity;

        // Destroy if particle is offscreen
        Box2 box = { position.position + sprite.offset, sprite.size };

        if (!Box2::overlaps(box, screenBox)) {
            scene.world.entities.registry.destroy(entity);
        }
    }
}

ParticleSystem::ParticleSystem() {
    gravity = 20.0f;
    explosionSpeedMin = 4.0f;
    explosionSpeedMax = 7.0f;
    explosionCountMin = 4;
    explosionCountMax = 7;
}