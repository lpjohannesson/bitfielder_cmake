#include "block_map_renderer.h"
#include "client/scenes/world_scene.h"
#include "client/client.h"
#include "components/block_renderer_component.h"

using namespace bf;

void BlockMapRenderer::renderBlock(const World &world, const BlockRenderData &renderData) {
    const entt::registry &blocksRegistry = world.blocks.registry;
    entt::entity block = world.blocks.getBlock(renderData.blockIndex);

    // Get renderer or skip
    if (!blocksRegistry.all_of<BlockRendererComponent>(block)) {
        return;
    }

    BlockRenderer *blockRenderer = blocksRegistry.get<BlockRendererComponent>(block).renderer;
    blockRenderer->render(renderData);
}

void BlockMapRenderer::createMesh(const World &world, BlockChunk &chunk) {
    int mapIndex = chunk.getMapIndex();
    int blockStartX = mapIndex * BlockChunk::SIZE.x;

    // Create block sample, including left and right chunks
    BlockSample<BlockChunk> blockSample(world.map, blockStartX - BlockChunk::SIZE.x, blockStartX + BlockChunk::SIZE.x);

    // Create render data
    BlockRenderData renderData;
    renderData.renderer = this;
    renderData.blockSample = &blockSample;

    for (int y = 0; y < BlockChunk::SIZE.y; y++) {
		for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            renderData.position = { blockStartX + x, y };
            
            // Get block data
			BlockData *blockData = chunk.getBlock({ x, y });

            // Render front
            renderData.blockIndex = blockData->frontIndex;
            renderData.spriteBatch = &frontSpriteBatch;
            renderData.onFrontLayer = true;

			renderBlock(world, renderData);

            // Render back
            renderData.blockIndex = blockData->backIndex;
            renderData.spriteBatch = &backSpriteBatch;
            renderData.onFrontLayer = false;

			renderBlock(world, renderData);
		}
	}

    // Get or create mesh
    BlockMesh *blockMesh = map.getChunk(mapIndex);

    if (blockMesh == nullptr) {
        blockMesh = &map.createChunk(mapIndex);
    }

    // Upload layers
    frontSpriteBatch.uploadMesh(blockMesh->frontMesh);
    backSpriteBatch.uploadMesh(blockMesh->backMesh);
}