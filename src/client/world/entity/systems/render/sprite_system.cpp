#include "sprite_system.h"
#include "client/scenes/world_scene.h"
#include "world/entity/components/flip_component.h"
#include "client/client.h"

using namespace bf;

void SpriteSystem::drawSprite(entt::entity entity, const SpriteComponent &sprite, const PositionComponent position, WorldScene &scene, Sprite &result) {
    entt::registry &entityRegistry = scene.world.entities.registry;

    result.uvBox = sprite.uvBox;

    bool flipX = false;

    if (entityRegistry.all_of<FlipComponent>(entity)) {
        flipX = entityRegistry.get<FlipComponent>(entity).flipX;
    }

    if (flipX) {
        result.box.start = { position.position.x + sprite.size.x, position.position.y };
        result.box.size = { -sprite.size.x, sprite.size.y };
    }
    else {
        result.box.start = position.position;
        result.box.size = sprite.size;
    }

    result.box.start += sprite.offset;
}

void SpriteSystem::render(WorldScene &scene) {
    auto view = scene.world.entities.registry.view<SpriteComponent, PositionComponent>();

    for (auto [entity, sprite, position] : view.each()) {
        drawSprite(entity, sprite, position, scene, spriteBatch.createSprite());
    }

    // Upload mesh
    spriteBatch.uploadMesh(mesh);
}

SpriteSystem::SpriteSystem() : mesh(client->spriteRenderer) {
    spriteBatch.defaultDepth = WorldRenderer::ENTITY_DEPTH;
}