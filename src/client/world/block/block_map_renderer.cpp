#include "block_map_renderer.h"
#include "client/world/world_renderer.h"

using namespace bf;

void BlockMapRenderer::createMesh(const WorldRenderer &renderer, World &world, entt::entity chunk) {
    entt::registry &mapRegistry = world.map.registry;
    entt::registry &blocksRegistry = world.blocks.registry;

    // Get chunk data from entity
    const BlockChunk *chunkData = mapRegistry.get<BlockChunk*>(chunk);

    for (int y = 0; y < BlockChunk::SIZE.y; y++) {
		for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            // Get block data for position
			glm::ivec2 position = { x, y };

            int index = chunkData->getIndex(position);
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

    // Get or create mesh
    BlockMesh* mesh;

    if (mapRegistry.all_of<BlockMesh*>(chunk)) {
        mesh = mapRegistry.get<BlockMesh*>(chunk);
    }
    else {
        mesh = new BlockMesh(renderer.spriteRenderer);
        meshes.push_back(mesh);

        mapRegistry.emplace<BlockMesh*>(chunk, mesh);
    }

    spriteBatch.uploadMesh(mesh->mesh);
}

void BlockMapRenderer::render(const WorldRenderer &renderer, const World &world) {
    // Get all chunk meshes
    auto meshView = world.map.registry.view<BlockMesh*>();

    for (const auto [chunk, blockMesh] : meshView.each()) {
        // Draw sprite mesh
        const SpriteMesh &mesh = blockMesh->mesh;
        renderer.spriteRenderer.renderMesh(mesh, renderer.textureAtlas.texture, renderer.getViewTransform());
    }
}

BlockMapRenderer::~BlockMapRenderer() {
    for (BlockMesh* mesh : meshes) {
        delete mesh;
    }

    meshes.clear();
}
