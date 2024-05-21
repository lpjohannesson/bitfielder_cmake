#include "block_map_renderer.h"
#include "client/world/world_renderer.h"

using namespace bf;

void BlockMapRenderer::createMesh(const WorldRenderer &renderer, const World &world, BlockChunk &chunk) {
    const entt::registry &blocksRegistry = world.blocks.registry;

    // Get chunk data from entity
    for (int y = 0; y < BlockChunk::SIZE.y; y++) {
		for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            // Get block data for position
			glm::ivec2 position = { x, y };

            int index = chunk.getIndex(position);
			entt::entity block = world.blocks.getBlock(index);

            // Get renderer or skip
            if (!blocksRegistry.all_of<BlockRenderer*>(block)) {
                continue;
            }

            BlockRenderer* blockRenderer = blocksRegistry.get<BlockRenderer*>(block);

            // Render block
            BlockRenderData renderData;
            renderData.renderer = this;
            renderData.renderPosition = position;

            blockRenderer->render(renderData);
		}
	}

    // Create mesh
    if (chunk.blockMesh == nullptr) {
        chunk.blockMesh = new BlockMesh(renderer.spriteRenderer);
    }

    spriteBatch.uploadMesh(chunk.blockMesh->mesh);
}

void BlockMapRenderer::render(const WorldRenderer &renderer, const World &world) {
    for (const auto &[chunkIndex, chunk] : world.map.chunks) {
        // Draw sprite mesh
        renderer.spriteRenderer.renderMesh(chunk.blockMesh->mesh, renderer.textureAtlas.texture);
    }
}
