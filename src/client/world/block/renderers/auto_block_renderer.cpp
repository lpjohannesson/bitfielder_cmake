#include "auto_block_renderer.h"
#include "client/scenes/world_scene.h"
#include "../components/block_sprite_set_component.h"
#include "../components/block_partial_component.h"
#include "../block_renderer_factory.h"

using namespace bf;

int AutoBlockRenderer::checkNeighbor(const BlockRenderData &data, glm::ivec2 offset) {
    const World &world = data.scene.world;
    const entt::registry &blocksRegistry = world.blocks.registry;

    BlockData *neighborBlockData = BlockChunk::getSampleBlock(data.position + offset, data.blockSample);

    if (neighborBlockData == nullptr) {
        return 0;
    }

    // Yes if same block on same layer
    int neighborBlockIndex;

    if (data.onFrontLayer) {
        neighborBlockIndex = neighborBlockData->getFrontIndex();
    }
    else {
        neighborBlockIndex = neighborBlockData->getBackIndex();
    }

    if (data.blockIndex == neighborBlockIndex) {
        return 1;
    }

    // Yes if front is not partial
    entt::entity neighborFrontBlock = world.blocks.getEntity(neighborBlockData->getFrontIndex());

    if (!blocksRegistry.all_of<BlockPartialComponent>(neighborFrontBlock)) {
        return 1;
    }

    // Yes if on back and back is not partial
    if (!data.onFrontLayer) {
        entt::entity neighborBackBlock = world.blocks.getEntity(neighborBlockData->getBackIndex());

        if (!blocksRegistry.all_of<BlockPartialComponent>(neighborBackBlock)) {
            return 1;
        }
    }

    return 0;
}

void AutoBlockRenderer::drawCorner(const BlockRenderData &renderData, int index, glm::vec2 offset, SpriteSet &sprites) {
    Sprite &sprite = renderData.spriteBatch->createSprite();
    
    sprite.box.start = glm::vec2(renderData.position) + offset;
    sprite.box.size = glm::vec2(0.5f);
    sprite.uvBox = sprites.boxes.at(index);
}

void AutoBlockRenderer::render(const BlockRenderData &renderData) {
    // Create bitmasks (horizontal, vertical, diagonal)
    int
        leftMask = checkNeighbor(renderData, { -1, 0 }) << SHIFT_HORIZONTAL,
        rightMask = checkNeighbor(renderData, { 1, 0 }) << SHIFT_HORIZONTAL,
        topMask = checkNeighbor(renderData, { 0, -1 }) << SHIFT_VERTICAL,
        bottomMask = checkNeighbor(renderData, { 0, 1 }) << SHIFT_VERTICAL,
        topLeftMask = checkNeighbor(renderData, { -1, -1 }) << SHIFT_DIAGONAL,
        topRightMask = checkNeighbor(renderData, { 1, -1 }) << SHIFT_DIAGONAL,
        bottomLeftMask = checkNeighbor(renderData, { -1, 1 }) << SHIFT_DIAGONAL,
        bottomRightMask = checkNeighbor(renderData, { 1, 1 }) << SHIFT_DIAGONAL;

    // Lookup frames with offsets per corner
    int
        topLeftFrame = frameStarts[leftMask | topMask | topLeftMask],
        topRightFrame = frameStarts[rightMask | topMask | topRightMask] + 1,
        bottomLeftFrame = frameStarts[leftMask | bottomMask | bottomLeftMask] + 10,
        bottomRightFrame = frameStarts[rightMask | bottomMask | bottomRightMask] + 11;
    
    // Draw corners
    World &world = renderData.scene.world;
    entt::entity block = world.blocks.getEntity(renderData.blockIndex);
    entt::registry &blocksRegistry = world.blocks.registry;
    
    SpriteSet &sprites = blocksRegistry.get<BlockSpriteSetComponent>(block).sprites;

    drawCorner(renderData, topLeftFrame, { 0.0f, 0.0f }, sprites);
    drawCorner(renderData, topRightFrame, { 0.5f, 0.0f }, sprites);
    drawCorner(renderData, bottomLeftFrame, { 0.0f, 0.5f }, sprites);
    drawCorner(renderData, bottomRightFrame, { 0.5f, 0.5f }, sprites);
}

void AutoBlockRenderer::createBlock(const BlockRendererFactoryData &data) {
    entt::registry &blocksRegistry = data.scene.world.blocks.registry;
    
    blocksRegistry.emplace<BlockAutoRendererComponent>(data.block, BlockAutoRendererComponent {});

    SpriteSet &sprites = blocksRegistry.emplace<BlockSpriteSetComponent>(
        data.block, BlockSpriteSetComponent {}).sprites;
    
    sprites.loadSprites(
        BlockRendererFactory::getBlockTexture(data.value, data.scene).uvBox, { 10, 2 });
}