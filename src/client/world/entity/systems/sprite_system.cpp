#include "sprite_system.h"
#include "client/scenes/world_scene.h"
#include "../components/sprite_component.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/sprite_flip_component.h"
#include "client/client.h"

using namespace bf;

void SpriteSystem::render(const WorldScene &scene) {
    const World &world = scene.world;
    const WorldRenderer &worldRenderer = scene.worldRenderer;

    auto view = world.entities.registry.view<SpriteComponent, PositionComponent>();

    for (auto [entity, sprite, position] : view.each()) {
        Sprite &batchSprite = spriteBatch.createSprite();

        batchSprite.uvBox = sprite.uvBox;
        batchSprite.color = sprite.color;

        bool flipX = false;

        if (world.entities.registry.all_of<SpriteFlipComponent>(entity)) {
            flipX = world.entities.registry.get<SpriteFlipComponent>(entity).flipX;
        }

        if (flipX) {
            batchSprite.box.start = { position.position.x + sprite.size.x, position.position.y };
            batchSprite.box.size = { -sprite.size.x, sprite.size.y };
        }
        else {
            batchSprite.box.start = position.position;
            batchSprite.box.size = sprite.size;
        }

        batchSprite.box.start += sprite.offset;
    }

    // Upload mesh
    spriteBatch.uploadMesh(mesh);
}