#pragma once
#include <glm/glm.hpp>
#include "gfx/core/texture_atlas.h"
#include "block/block_map_renderer.h"
#include "entity/entity_renderer.h"
#include "shadow_buffer.h"

namespace bf {
    class WorldScene;

	class WorldRenderer {
    private:
        glm::mat4 viewTransform, shadowTransform;

    public:
        TextureAtlas textureAtlas;

        SpriteProgram frontSpriteProgram, backSpriteProgram, shadowSpriteProgram;
        ShadowBuffer shadowBuffer;

        BlockMapRenderer map;
        EntityRenderer entities;

        void updateSize(glm::ivec2 size);

        void render(const WorldScene &scene);

        WorldRenderer(WorldScene &scene);
	};
}