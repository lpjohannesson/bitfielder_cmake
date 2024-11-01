#pragma once
#include "gfx/sprite/sprite_batch.h"
#include "world/block/block_map.h"
#include "world/block/block_chunk.h"
#include "block_mesh.h"
#include "block_render_data.h"

namespace bf {
    class WorldScene;

    struct BlockLightRenderData {
        glm::vec3 color;
        bool occluded;
    };

    class BlockMapRenderer {
    private:
        static constexpr glm::ivec2 lightOffsets[] = {
            { -1, -1 },
            { 0, -1 },
            { 1, -1 },
            { -1, 0 },
            { 1, 0 },
            { -1, 1 },
            { 0, 1 },
            { 1, 1 }
        };

        static constexpr int neighborIndicesTable[4][3] = {
            { 3, 1, 0 },
            { 4, 1, 2 },
            { 3, 6, 5 },
            { 4, 6, 7 }
        };

        static bool isBlockOccluded(const BlockData &blockData, const WorldScene &scene);
        static BlockLightRenderData getLightData(const BlockData &blockData);

        static void drawLightSprites(
            glm::vec2 position,
            const BlockLightRenderData center,
            const BlockLightRenderData neighbors[],
            SpriteBatch &lightSpriteBatch,
            SpriteBatch &aoSpriteBatch);

        void renderBlock(const BlockRenderData &data);

    public:
        BlockMap<BlockMesh> map;
        SpriteBatch frontSpriteBatch, backSpriteBatch, lightSpriteBatch, aoSpriteBatch;

        void createMesh(
            BlockChunk &chunk,
            WorldScene &scene,
            int sectionStart = 0,
            int sectionEnd = BlockMesh::SECTION_COUNT - 1);

        BlockMapRenderer();
    };
}