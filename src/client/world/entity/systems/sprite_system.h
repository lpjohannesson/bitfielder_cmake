#pragma once
#include "render_system.h"
#include "gfx/sprite/sprite_batch.h"
#include "client/client.h"

namespace bf {
	class SpriteSystem : public RenderSystem {
    public:
        SpriteBatch spriteBatch;
        SpriteMesh mesh;

        void render(const WorldScene &scene) override;

        inline SpriteSystem() : mesh(client->spriteRenderer) {}
	};
}