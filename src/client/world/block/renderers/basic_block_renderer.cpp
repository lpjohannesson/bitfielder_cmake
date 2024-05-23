#include "basic_block_renderer.h"
#include "block_renderer_impl.h"

using namespace bf;

void BasicBlockRenderer::render(const BlockRenderData &renderData) {
    sprite.box.start = renderData.position;
    renderData.renderer->spriteBatch.drawSprite(sprite);
}