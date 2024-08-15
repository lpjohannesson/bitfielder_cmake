#include "auto_block_renderer.h"
#include "client/scenes/world_scene.h"
#include "../components/block_sprite_component.h"
#include "../components/block_frames_component.h"
#include "../components/block_partial_component.h"
#include "../block_renderer_factory.h"

using namespace bf;

int AutoBlockRenderer::checkNeighbor(const BlockRenderData &renderData, glm::ivec2 offset) {
    const World &world = renderData.scene->world;
    const entt::registry &blocksRegistry = world.blocks.registry;

    BlockData *neighborBlockData = BlockChunk::getSampleBlock(*renderData.blockSample, renderData.position + offset);

    if (neighborBlockData == nullptr) {
        return 0;
    }

    // Yes if same block on same layer
    int neighborBlockIndex;

    if (renderData.onFrontLayer) {
        neighborBlockIndex = neighborBlockData->getFrontIndex();
    }
    else {
        neighborBlockIndex = neighborBlockData->getBackIndex();
    }

    if (renderData.blockIndex == neighborBlockIndex) {
        return 1;
    }

    // Yes if front is not partial
    entt::entity neighborFrontBlock = world.blocks.getEntity(neighborBlockData->getFrontIndex());

    if (!blocksRegistry.all_of<BlockPartialComponent>(neighborFrontBlock)) {
        return 1;
    }

    // Yes if on back and back is not partial
    if (!renderData.onFrontLayer) {
        entt::entity neighborBackBlock = world.blocks.getEntity(neighborBlockData->getBackIndex());

        if (!blocksRegistry.all_of<BlockPartialComponent>(neighborBackBlock)) {
            return 1;
        }
    }

    return 0;
}

void AutoBlockRenderer::drawCorner(const BlockRenderData &renderData, int frame, glm::vec2 offset, Sprite &sprite, SpriteFrames &frames) {
    sprite.uvBox = frames.frames.at(frame);
    sprite.box.start = glm::vec2(renderData.position) + offset;
    renderData.spriteBatch->drawSprite(sprite);
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
        topLeftFrame = frameStartLookup[leftMask | topMask | topLeftMask],
        topRightFrame = frameStartLookup[rightMask | topMask | topRightMask] + 1,
        bottomLeftFrame = frameStartLookup[leftMask | bottomMask | bottomLeftMask] + 10,
        bottomRightFrame = frameStartLookup[rightMask | bottomMask | bottomRightMask] + 11;
    
    // Draw corners
    World &world = renderData.scene->world;
    entt::entity block = world.blocks.getEntity(renderData.blockIndex);
    entt::registry &blocksRegistry = world.blocks.registry;

    Sprite &sprite = blocksRegistry.get<BlockSpriteComponent>(block).sprite;
    SpriteFrames &frames = blocksRegistry.get<BlockFramesComponent>(block).frames;

    drawCorner(renderData, topLeftFrame, { 0.0f, 0.0f }, sprite, frames);
    drawCorner(renderData, topRightFrame, { 0.5f, 0.0f }, sprite, frames);
    drawCorner(renderData, bottomLeftFrame, { 0.0f, 0.5f }, sprite, frames);
    drawCorner(renderData, bottomRightFrame, { 0.5f, 0.5f }, sprite, frames);
}

AutoBlockRenderer::AutoBlockRenderer(const rapidjson::Value &value, entt::entity block, WorldScene &scene) {
    entt::registry &blocksRegistry = scene.world.blocks.registry;
    Sprite &sprite = blocksRegistry.emplace<BlockSpriteComponent>(block, BlockSpriteComponent {}).sprite;
    SpriteFrames &frames = blocksRegistry.emplace<BlockFramesComponent>(block, BlockFramesComponent {}).frames;

    sprite.box.size = glm::vec2(0.5f);

    frames.loadFrames(BlockRendererFactory::getBlockTexture(value, scene).uvBox, { 10, 2 });
}