#include "block_map_renderer.h"
#include "client/scenes/world_scene.h"
#include <glm/gtx/easing.hpp>
#include "block_render_data.h"
#include "client/client.h"
#include "renderers/basic_block_renderer.h"
#include "renderers/auto_block_renderer.h"
#include "components/block_partial_component.h"

using namespace bf;

bool BlockMapRenderer::isBlockOccluded(const BlockData &blockData, const WorldScene &scene) {
    // Get whether front is not partial
    entt::entity block = scene.world.blocks.getEntity(blockData.getFrontIndex());
    return !scene.world.blocks.registry.all_of<BlockPartialComponent>(block);
}

BlockLightRenderData BlockMapRenderer::getLightData(const BlockData &blockData) {
    // Get colour
    int sunlight = blockData.getSunlight();

    glm::ivec3 sourceLight = {
        blockData.getRedLight(),
        blockData.getGreenLight(),
        blockData.getBlueLight()
    };

    glm::ivec3 lightValue = glm::max(sourceLight, glm::ivec3(sunlight));
    glm::vec3 color = glm::vec3(lightValue) / 15.0f;

    color = glm::vec3(
        glm::quadraticEaseOut(color.r),
        glm::quadraticEaseOut(color.g),
        glm::quadraticEaseOut(color.b));

    return { color, false };
}

void BlockMapRenderer::drawLightSprites(
    glm::vec2 position,
    const BlockLightRenderData center,
    const BlockLightRenderData neighbors[],
    SpriteBatch &lightSpriteBatch,
    SpriteBatch &aoSpriteBatch) {
    
    // Create sprites
    Sprite lightSprite, aoSprite;

    lightSprite.box.start = aoSprite.box.start = position;
    lightSprite.box.size = aoSprite.box.size = glm::vec2(1.0f);
    
    for (int i = 0; i < 4; i++) {
        // Get neighbours for each corner
        const int (&neighborIndices)[3] = neighborIndicesTable[i];

        // Draw average color
        glm::vec3 color = center.color;
        
        for (int neighborIndex : neighborIndices) {
            color += neighbors[neighborIndex].color;
        }

        color *= 0.25f;

        lightSprite.cornerColors[i] = glm::vec4(color, 1.0f);

        // Draw ambient occlusion
        if (!center.occluded) {
            float ao = 0.0f;

            for (int neighborIndex : neighborIndices) {
                ao += (float)neighbors[neighborIndex].occluded;
            }

            ao *= 0.1f;
            aoSprite.cornerColors[i] = glm::vec4(glm::vec3(1.0f - ao), 1.0f);
        }
    }

    // Draw sprites
    // TODO: Check for blank sprites
    lightSpriteBatch.createSprite() = lightSprite;
    aoSpriteBatch.createSprite() = aoSprite;
}

void BlockMapRenderer::renderBlock(const BlockRenderData &data) {
    const World &world = data.scene.world;
    const entt::registry &blocksRegistry = world.blocks.registry;

    entt::entity block = world.blocks.getEntity(data.blockIndex);

    if (blocksRegistry.all_of<BlockBasicRendererComponent>(block)) {
        BasicBlockRenderer::render(data);
        return;
    }

    if (blocksRegistry.all_of<BlockAutoRendererComponent>(block)) {
        AutoBlockRenderer::render(data);
        return;
    }
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
                renderData.onFrontLayer = true;
                renderData.spriteBatch = &frontSpriteBatch;

                renderBlock(renderData);

                // Render back
                renderData.blockIndex = blockData.getBackIndex();
                renderData.onFrontLayer = false;
                renderData.spriteBatch = &backSpriteBatch;

                renderBlock(renderData);

                // Render light
                BlockLightRenderData lightCenter = getLightData(blockData);
                lightCenter.occluded = isBlockOccluded(blockData, scene);

                // Occlude center if back block is partial
                entt::entity backBlock = scene.world.blocks.getEntity(blockData.getBackIndex());
                lightCenter.occluded |= scene.world.blocks.registry.all_of<BlockPartialComponent>(backBlock);

                // Get light neighbors
                BlockLightRenderData lightNeighbors[8];

                for (int i = 0; i < 8; i++) {
                    glm::ivec2 neighborPosition = position + lightOffsets[i];
                    BlockData *neighborBlockData = BlockChunk::getSampleBlock(neighborPosition, blockSample);

                    BlockLightRenderData &lightNeighbor = lightNeighbors[i];

                    // Default to white on no neighbor
                    if (neighborBlockData == nullptr) {
                        lightNeighbor = { glm::vec3(1.0f), false };
                        continue;
                    }

                    lightNeighbor = getLightData(*neighborBlockData);

                    if (!lightCenter.occluded) {
                        lightNeighbor.occluded = isBlockOccluded(*neighborBlockData, scene);
                    }
                }

                drawLightSprites(
                    position, lightCenter, lightNeighbors, lightSpriteBatch, aoSpriteBatch);
            }
        }

        // Upload layers
        BlockMeshSection &section = mesh->sections[sectionIndex];

        frontSpriteBatch.uploadMesh(section.frontMesh);
        backSpriteBatch.uploadMesh(section.backMesh);
        lightSpriteBatch.uploadMesh(section.lightMesh);
        aoSpriteBatch.uploadMesh(section.aoMesh);
    }
}

BlockMapRenderer::BlockMapRenderer() {
    frontSpriteBatch.defaultDepth = WorldRenderer::BLOCK_FRONT_DEPTH;
    backSpriteBatch.defaultDepth = WorldRenderer::BLOCK_BACK_DEPTH;
    lightSpriteBatch.defaultDepth = WorldRenderer::BLOCK_LIGHT_DEPTH;
    aoSpriteBatch.defaultDepth = WorldRenderer::BLOCK_AO_DEPTH;
}