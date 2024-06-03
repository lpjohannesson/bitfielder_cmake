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

    // On back and back block is not air
    if (!renderData.onFrontLayer && neighborBlock->backIndex != 0) {
        return 1;
    }

    return 0;
}

void AutoBlockRenderer::drawCorner(const BlockRenderData &renderData, int frame, glm::vec2 offset) {
    sprite.uvBox = spriteFrames.frames.at(frame);
    sprite.box.start = glm::vec2(renderData.position) + offset;
    renderData.spriteBatch->drawSprite(sprite);
}

void AutoBlockRenderer::loadFrames(Box2 uvBox) {
    spriteFrames.loadFrames(uvBox, { 10, 2 });
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
    drawCorner(renderData, topLeftFrame, { 0.0f, 0.0f });
    drawCorner(renderData, topRightFrame, { 0.5f, 0.0f });
    drawCorner(renderData, bottomLeftFrame, { 0.0f, 0.5f });
    drawCorner(renderData, bottomRightFrame, { 0.5f, 0.5f });
}

AutoBlockRenderer::AutoBlockRenderer() {
    sprite.box.size = glm::vec2(0.5f);
}