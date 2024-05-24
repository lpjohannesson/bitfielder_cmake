#pragma once
#include "gfx/sprite/sprite_mesh.h"
#include "client/client.h"

namespace bf {
    class BlockMesh {
    public:
        SpriteMesh mesh;

        inline BlockMesh() : mesh(client->spriteRenderer) {}
    };
}