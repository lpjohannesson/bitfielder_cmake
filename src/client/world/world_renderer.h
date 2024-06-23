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
        glm::mat4 shadowTransform, viewTransform, shadowViewTransform;
        Box2 screenBox;

        void loadTextureAtlas();

    public:
        static constexpr float SHADOW_OFFSET = 2.0f / 16.0f;

        TextureAtlas textureAtlas;
        Texture texture;

        SpriteProgram backSpriteProgram, shadowSpriteProgram;
        ShadowBuffer shadowBuffer;

        BlockMapRenderer map;
        EntityRenderer entities;

        glm::vec4 backgroundColor;

        inline Box2 getScreenBox() const { return screenBox; }

        void updateTransforms(const WorldScene &scene);
        void updateSize(glm::ivec2 size, const WorldScene &scene);

        void render(const WorldScene &scene);

        WorldRenderer(WorldScene &scene);
	};
}