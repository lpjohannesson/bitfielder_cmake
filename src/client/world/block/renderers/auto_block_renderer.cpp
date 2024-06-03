#include "auto_block_renderer.h"
#include "block_renderer_impl.h"

using namespace bf;

int AutoBlockRenderer::checkNeighbor(const BlockRenderData &renderData, glm::ivec2 offset) {
    BlockData *neighborBlock = BlockChunk::getSampleBlock(*renderData.blockSample, renderData.position + offset);

    if (neighborBlock == nullptr) {
        return 0;
    }

    // Front block is not air
    if (neighborBlock->frontIndex != 0) {
        return 1;
    }

    // On back and black block is not air
    if (!renderData.onFrontLayer && neighborBlock->backIndex != 0) {
        return 1;
    }

    return 0;
}

void AutoBlockRenderer::loadFrames(Box2 uvBox) {
    spriteFrames.loadFrames(uvBox, { 8, 6 });
}

void AutoBlockRenderer::render(const BlockRenderData &renderData) {
    // Check neighbours
    int bitmask = 0;

    bitmask |= checkNeighbor(renderData, { -1, -1 }) << 0;
    bitmask |= checkNeighbor(renderData, { 0, -1 }) << 1;
    bitmask |= checkNeighbor(renderData, { 1, -1 }) << 2;
    bitmask |= checkNeighbor(renderData, { -1, 0 }) << 3;
    bitmask |= checkNeighbor(renderData, { 1, 0 }) << 4;
    bitmask |= checkNeighbor(renderData, { -1, 1 }) << 5;
    bitmask |= checkNeighbor(renderData, { 0, 1 }) << 6;
    bitmask |= checkNeighbor(renderData, { 1, 1 }) << 7;

    int frameIndex = bitmaskTable[bitmask];
    sprite.uvBox = spriteFrames.frames.at(frameIndex);

    sprite.box.start = renderData.position;
    renderData.spriteBatch->drawSprite(sprite);
}