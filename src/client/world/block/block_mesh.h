#pragma once
#include "world/block/block_map_element.h"
#include "gfx/sprite/sprite_mesh.h"
#include "client/client.h"

namespace bf {
    class BlockMesh : public BlockMapElement {
    public:
        SpriteMesh mesh;

        inline BlockMesh(int mapIndex) : BlockMapElement(mapIndex), mesh(client->spriteRenderer) {}
    };
}