#include "basic_block_renderer.h"
#include "block_renderer_impl.h"

using namespace bf;

void BasicBlockRenderer::render(const BlockRenderData &renderData) {
    // Draw sprite at position
    sprite.box.start = renderData.position;
    renderData.renderer->spriteBatch.drawSprite(sprite);
}