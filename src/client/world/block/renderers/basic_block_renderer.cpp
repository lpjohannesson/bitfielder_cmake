#include "basic_block_renderer.h"
#include "client/scenes/world_scene.h"
#include "../components/block_texture_component.h"
#include "../block_renderer_factory.h"

using namespace bf;

void BasicBlockRenderer::render(const BlockRenderData &renderData) {
    World &world = renderData.scene.world;
    entt::entity block = world.blocks.getEntity(renderData.blockIndex);
    BlockTextureComponent &blockTexture = world.blocks.registry.get<BlockTextureComponent>(block);

    // Draw sprite at position
    Sprite &sprite = renderData.spriteBatch->createSprite();

    sprite.box.start = renderData.position;
    sprite.box.size = glm::vec2(1.0f);
    sprite.uvBox = blockTexture.uvBox;
}

BasicBlockRenderer::BasicBlockRenderer(const rapidjson::Value &value, entt::entity block, WorldScene &scene) {
    entt::registry &blocksRegistry = scene.world.blocks.registry;
    BlockTextureComponent &blockTexture = blocksRegistry.emplace<BlockTextureComponent>(block, BlockTextureComponent {});

    blockTexture.uvBox = BlockRendererFactory::getBlockTexture(value, scene).uvBox;
}