#pragma once
#include <glm/glm.hpp>
#include "core/box2.h"
#include "gfx/core/texture_atlas.h"
#include "block/block_map_renderer.h"
#include "entity/entity_renderer.h"
#include "shadow_buffer.h"

namespace bf {
    class WorldScene;

	class WorldRenderer {
    private:
        glm::mat4 windowTransform, shadowTransform, viewTransform, shadowViewTransform;
        Box2 screenBox;

        void updateTransforms(const WorldScene &scene);

    public:
        Box2 getScreenBox() const { return screenBox; }

        TextureAtlas textureAtlas;

        SpriteProgram frontSpriteProgram, backSpriteProgram, shadowSpriteProgram;
        ShadowBuffer shadowBuffer;

        BlockMapRenderer map;
        EntityRenderer entities;

        void updateSize(glm::ivec2 size, const WorldScene &scene);

        void render(const WorldScene &scene);

        WorldRenderer(WorldScene &scene);
	};
}