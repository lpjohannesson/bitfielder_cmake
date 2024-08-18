#include "hud.h"
#include "client/scenes/world_scene.h"
#include "item/components/item_texture_component.h"
#include "world/entity/components/inventory_component.h"

using namespace bf;

void HUD::updateMesh(WorldScene &scene) {
    // Draw item bar
    glm::vec2 itemBarCorner = client->getMenuInverseTransform() * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
    glm::vec2 itemBarStart = { itemBarCorner.x + 4.0f, itemBarCorner.y - itemBarSprite.box.size.y - 8.0f };

    itemBarSprite.box.start = itemBarStart;
    spriteBatch.createSprite() = itemBarSprite;

    // Draw items
    entt::entity player = scene.clientContent.player;
    Inventory &inventory = scene.world.entities.registry.get<InventoryComponent>(player).inventory;
    entt::registry &itemsRegistry = scene.world.items.registry;

    glm::vec2 itemsStart = itemBarStart + glm::vec2(3.0f);
    glm::vec2 itemSize = glm::vec2(16.0f);

    for (int i = 0; i < 10; i++) {
        // Get item or skip
        ItemSlot &itemSlot = inventory.items[i];
        int itemIndex = itemSlot.itemIndex - 1;

        if (itemIndex < 0) {
            continue;
        }

        entt::entity item = scene.world.items.getEntity(itemIndex);

        if (!itemsRegistry.all_of<ItemTextureComponent>(item)) {
            continue;
        }
        
        // Draw item
        Sprite &itemSprite = spriteBatch.createSprite();

        itemSprite.box.start = { itemsStart.x + i * itemSize.x, itemsStart.y };
        itemSprite.box.size = itemSize;

        itemSprite.uvBox = itemsRegistry.get<ItemTextureComponent>(item).uvBox;
    }

    // Draw arrow
    itemArrowSprite.box.start = { itemsStart.x + inventory.selectedIndex * itemSize.x, itemBarStart.y - 4.0f };
    spriteBatch.createSprite() = itemArrowSprite;

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

    TextureSection itemArrowTexture = scene.worldRenderer.textureAtlas.getSection("assets/textures/world/hud/item_arrow.png");

    itemArrowSprite.box.size = itemArrowTexture.box.size;
    itemArrowSprite.uvBox = itemArrowTexture.uvBox;
}