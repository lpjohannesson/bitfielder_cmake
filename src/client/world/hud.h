#pragma once
#include "gfx/sprite/sprite_mesh.h"
#include "gfx/sprite/sprite_batch.h"
#include "client/client.h"

namespace bf {
    class WorldScene;

    class HUD {
    public:
        SpriteMesh mesh;

        SpriteBatch spriteBatch;
        Sprite itemBarSprite, itemArrowSprite;

        void updateMesh(WorldScene &scene);
        void render(WorldScene &scene);

        void start(WorldScene &scene);

        inline HUD() : mesh(client->spriteRenderer) {}
    };
}