#pragma once
#include "gfx/sprite/sprite_mesh.h"

namespace bf {
    class BlockMesh {
    public:
        SpriteMesh mesh;

        inline BlockMesh(const SpriteRenderer &renderer) : mesh(renderer) {}
    };
}