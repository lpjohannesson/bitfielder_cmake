#pragma once
#include <glm/glm.hpp>
#include "gfx/core/texture_atlas.h"
#include "block/block_map_renderer.h"
#include "entity/entity_renderer.h"

namespace bf {
    class WorldScene;

	class WorldRenderer {
    private:
        glm::mat4 viewTransform;

    public:
        TextureAtlas textureAtlas;

        BlockMapRenderer map;
        EntityRenderer entities;

        void updateSize(glm::ivec2 size);

        void render(const WorldScene &scene);
	};
}