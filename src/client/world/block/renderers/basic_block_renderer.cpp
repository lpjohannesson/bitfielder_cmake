#include "basic_block_renderer.h"
#include "client/scenes/world_scene.h"
#include "../components/block_sprite_component.h"
#include "../block_renderer_factory.h"

using namespace bf;

void BasicBlockRenderer::render(const BlockRenderData &renderData) {
    World &world = renderData.scene->world;
    entt::entity block = world.blocks.getEntity(renderData.blockIndex);
    Sprite &sprite = world.blocks.registry.get<BlockSpriteComponent>(block).sprite;

    // Draw sprite at position
    sprite.box.start = renderData.position;
    renderData.spriteBatch->drawSprite(sprite);
}

BasicBlockRenderer::BasicBlockRenderer(const rapidjson::Value &value, entt::entity block, WorldScene &scene) {
    entt::registry &blocksRegistry = scene.world.blocks.registry;
    Sprite &sprite = blocksRegistry.emplace<BlockSpriteComponent>(block, BlockSpriteComponent {}).sprite;

    sprite.box.size = glm::vec2(1.0f);
    sprite.uvBox = BlockRendererFactory::getBlockTexture(value, scene).uvBox;
}