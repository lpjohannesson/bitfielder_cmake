#pragma once
#include "world/block/block_map_element.h"
#include "world/block/block_chunk.h"
#include "gfx/sprite/sprite_mesh.h"
#include "client/client.h"

namespace bf {
    class BlockMeshSection {
    public:
        SpriteMesh frontMesh, backMesh, lightMesh;

        inline BlockMeshSection() :
            frontMesh(client->spriteRenderer),
            backMesh(client->spriteRenderer),
            lightMesh(client->spriteRenderer) {}
    };
    
    class BlockMesh : public BlockMapElement {
    public:
        static constexpr glm::ivec2 SECTION_SIZE = { 16, 16 };
        static constexpr int SECTION_COUNT = BlockChunk::SIZE.y / SECTION_SIZE.y;

        BlockMeshSection sections[SECTION_COUNT];

        static int getSectionIndex(int blockY);

        inline BlockMesh(int mapIndex) : BlockMapElement(mapIndex) {}
    };
}