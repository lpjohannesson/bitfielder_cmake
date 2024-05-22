#include "block_map_renderer.h"
#include "world/world.h"

using namespace bf;

void BlockMapRenderer::createMesh(const World &world, BlockChunk &chunk) {
    const entt::registry &blocksRegistry = world.blocks.registry;

    int blockStartX = chunk.getMapIndex() * BlockChunk::SIZE.x;

    for (int y = 0; y < BlockChunk::SIZE.y; y++) {
		for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            // Get block data for position
			glm::ivec2 position = { x, y };

            int index = chunk.getBlockIndex(position);
			entt::entity block = world.blocks.getBlock(index);

            // Get renderer or skip
            if (!blocksRegistry.all_of<BlockRenderer*>(block)) {
                continue;
            }

            BlockRenderer* blockRenderer = blocksRegistry.get<BlockRenderer*>(block);

            // Render block
            BlockRenderData renderData;
            renderData.renderer = this;
            renderData.position = { blockStartX + x, y };

            blockRenderer->render(renderData);
		}
	}

    // Create mesh
    if (chunk.blockMesh == nullptr) {
        chunk.blockMesh = new BlockMesh(world.renderer.spriteRenderer);
    }

    spriteBatch.uploadMesh(chunk.blockMesh->mesh);
}

void BlockMapRenderer::render(const World &world) {
    for (const auto &[chunkIndex, chunk] : world.map.chunks) {
        // Draw sprite mesh
        if (chunk.blockMesh == nullptr) {
            continue;
        }

        world.renderer.spriteRenderer.renderMesh(
            chunk.blockMesh->mesh, world.renderer.textureAtlas.texture);
    }
}
