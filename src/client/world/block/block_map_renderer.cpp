#include "block_map_renderer.h"
#include "client/scenes/world_scene.h"
#include "client/client.h"
#include "components/block_renderer_component.h"

using namespace bf;

void BlockMapRenderer::renderBlock(const BlockRenderData &renderData) {
    const World &world = renderData.scene.world;
    const entt::registry &blocksRegistry = world.blocks.registry;

    entt::entity block = world.blocks.getEntity(renderData.blockIndex);

    // Get renderer or skip
    if (!blocksRegistry.all_of<BlockRendererComponent>(block)) {
        return;
    }

    BlockRenderer *blockRenderer = blocksRegistry.get<BlockRendererComponent>(block).renderer;
    blockRenderer->render(renderData);
}

void BlockMapRenderer::createMesh(BlockChunk &chunk, WorldScene &scene, int sectionStart, int sectionEnd) {
    int mapIndex = chunk.getMapIndex();
    int blockStartX = mapIndex * BlockChunk::SIZE.x;

    // Create block sample, including left and right chunks
    BlockSample<BlockChunk> blockSample;
    blockSample.sampleChunks(scene.world.map, mapIndex - 1, mapIndex + 1);

    // Create render data
    BlockRenderData renderData{ scene, blockSample };

    // Get or create mesh
    BlockMesh *mesh = map.getChunk(mapIndex);

    if (mesh == nullptr) {
        mesh = &map.createChunk(mapIndex);
    }

    for (int sectionIndex = sectionStart; sectionIndex <= sectionEnd; sectionIndex++) {
        // Draw vertical sections
        int sectionY = sectionIndex * BlockMesh::SECTION_SIZE.y;

        // Create light sprites
        for (int y = 0; y < BlockMesh::SECTION_SIZE.y; y++) {
            for (int x = 0; x < BlockMesh::SECTION_SIZE.x; x++) {
                // Get positions
                int blockY = sectionY + y;

                glm::ivec2
                    position = { blockStartX + x, blockY },
                    chunkPosition = { x, blockY };
                
                renderData.position = position;
                
                // Get block data
                BlockData &blockData = chunk.getBlock(chunkPosition);

                // Render front
                renderData.blockIndex = blockData.getFrontIndex();
                renderData.spriteBatch = &frontSpriteBatch;
                renderData.onFrontLayer = true;

                renderBlock(renderData);

                // Render back
                renderData.blockIndex = blockData.getBackIndex();
                renderData.spriteBatch = &backSpriteBatch;
                renderData.onFrontLayer = false;

                renderBlock(renderData);

                // Render light
                int light = blockData.getSunlight();

                if (light < 15) {
                    Sprite &lightSprite = lightSpriteBatch.createSprite();
                    
                    lightSprite.box.start = position;
                    lightSprite.box.size = glm::vec2(1.0f);
                    lightSprite.color = glm::vec4(glm::vec3((float)light / 15.0f), 0.0f);
                }
            }
        }

        // Upload layers
        BlockMeshSection &section = mesh->sections[sectionIndex];

        frontSpriteBatch.uploadMesh(section.frontMesh);
        backSpriteBatch.uploadMesh(section.backMesh);
        lightSpriteBatch.uploadMesh(section.lightMesh);
    }
}