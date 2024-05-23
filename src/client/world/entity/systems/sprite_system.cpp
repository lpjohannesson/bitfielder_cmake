#include "sprite_system.h"
#include "render_system_impl.h"
#include "../components/sprite_component.h"

using namespace bf;

void SpriteSystem::render(const World &world) {
    // Draw all entities with sprites
    auto view = world.entities.registry.view<const SpriteComponent>();

    for (auto [entity, spriteComponent] : view.each()) {
        spriteBatch.drawSprite(spriteComponent.sprite);
    }

    // Upload and render
    spriteBatch.uploadMesh(mesh);

    world.renderer.spriteRenderer.renderMesh(
        mesh, world.renderer.textureAtlas.texture);
}