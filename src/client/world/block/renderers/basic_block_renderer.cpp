#include "basic_block_renderer.h"
#include "client/scenes/world_scene.h"
#include "../components/block_texture_component.h"
#include "../block_renderer_factory.h"

using namespace bf;

void BasicBlockRenderer::render(const BlockRenderData &data) {
    World &world = data.scene.world;
    entt::entity block = world.blocks.getEntity(data.blockIndex);
    BlockTextureComponent &blockTexture = world.blocks.registry.get<BlockTextureComponent>(block);

    // Draw sprite at position
    Sprite &sprite = data.spriteBatch->createSprite();

    sprite.box.start = data.position;
    sprite.box.size = glm::vec2(1.0f);
    sprite.uvBox = blockTexture.uvBox;
}

void BasicBlockRenderer::createBlock(const BlockRendererFactoryData &data) {
    entt::registry &blocksRegistry = data.scene.world.blocks.registry;
    
    blocksRegistry.emplace<BlockBasicRendererComponent>(data.block, BlockBasicRendererComponent {});

    blocksRegistry.emplace<BlockTextureComponent>(data.block,
        BlockTextureComponent { BlockRendererFactory::getBlockTexture(data.value, data.scene).uvBox });
}