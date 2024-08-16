#include "hud.h"
#include "client/scenes/world_scene.h"
#include "world/entity/components/inventory_component.h"

using namespace bf;

void HUD::updateMesh(WorldScene &scene) {
    // Draw item bar
    glm::vec2 itemBarCorner = client->getMenuInverseTransform() * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
    itemBarSprite.box.start = itemBarCorner - itemBarSprite.box.size - 2.0f;

    Sprite &sprite = spriteBatch.createSprite();
    sprite = itemBarSprite;

    // Draw items
    entt::entity player = scene.clientContent.player;
    Inventory &inventory = scene.world.entities.registry.get<InventoryComponent>(player).inventory;

    spriteBatch.uploadMesh(mesh);
}

void HUD::render(WorldScene &scene) {
    client->spriteProgram.setTransform(client->getMenuTransform());
    client->spriteRenderer.renderMesh(mesh, client->spriteProgram, scene.worldRenderer.texture);
}

void HUD::start(WorldScene &scene) {
    TextureSection itemBarTexture = scene.worldRenderer.textureAtlas.getSection("assets/textures/world/hud/item_bar.png");

    itemBarSprite.box.size = itemBarTexture.box.size;
    itemBarSprite.uvBox = itemBarTexture.uvBox;

    updateMesh(scene);
}