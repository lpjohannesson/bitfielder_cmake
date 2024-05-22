#pragma once
#include <glm/glm.hpp>
#include "gfx/sprite/sprite_renderer.h"
#include "gfx/core/texture_atlas.h"
#include "block/block_map_renderer.h"

namespace bf {
    class World;

	class WorldRenderer {
    private:
        glm::mat4 viewTransform;

    public:
        inline glm::mat4 getViewTransform() const { return viewTransform; }

        SpriteRenderer spriteRenderer;
        TextureAtlas textureAtlas;
        BlockMapRenderer map;

        void updateSize(glm::ivec2 size);

        void render(const World &world); 
	};
}