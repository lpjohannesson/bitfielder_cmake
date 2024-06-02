#pragma once
#include "world/block/block_map_element.h"
#include "gfx/sprite/sprite_mesh.h"
#include "client/client.h"

namespace bf {
    // TODO: Sections
    class BlockMesh : public BlockMapElement {
    public:
        SpriteMesh frontMesh, backMesh;

        inline BlockMesh(int mapIndex) : BlockMapElement(mapIndex),
            frontMesh(client->spriteRenderer),
            backMesh(client->spriteRenderer) {}
    };
}