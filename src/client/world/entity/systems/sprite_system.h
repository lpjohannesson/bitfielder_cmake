#pragma once
#include "render_system.h"
#include "gfx/sprite/sprite_batch.h"

namespace bf {
	class SpriteSystem : public RenderSystem {
    public:
        SpriteBatch spriteBatch;
        SpriteMesh mesh;

        void render(const World &world) override;

        inline SpriteSystem(const SpriteRenderer &renderer) : mesh(renderer) {}
	};
}