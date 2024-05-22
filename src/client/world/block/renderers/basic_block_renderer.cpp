#include "basic_block_renderer.h"

using namespace bf;

void BasicBlockRenderer::render(const BlockRenderData &renderData) {
    sprite.box.start = renderData.position;
    renderData.renderer->spriteBatch.drawSprite(sprite);
}