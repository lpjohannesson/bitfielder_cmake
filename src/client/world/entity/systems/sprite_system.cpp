#include "sprite_system.h"
#include "render_system_impl.h"
#include "../components/sprite_component.h"
#include "world/entity/components/position_component.h"
#include "client/client.h"

using namespace bf;

void SpriteSystem::render(const WorldScene &scene) {
    const World &world = scene.world;
    const WorldRenderer &worldRenderer = scene.worldRenderer;

    auto view = world.entities.registry.view<SpriteComponent, PositionComponent>();

    for (auto [entity, sprite, position] : view.each()) {
        // Draw entity sprite
        Sprite batchSprite;
        batchSprite.box.start = position.position;
        batchSprite.box.size = sprite.size;
        batchSprite.uvBox = sprite.uvBox;

        spriteBatch.drawSprite(batchSprite);
    }

    // Upload and render
    spriteBatch.uploadMesh(mesh);

    client->spriteRenderer.renderMesh(
        mesh, worldRenderer.textureAtlas.texture);
}