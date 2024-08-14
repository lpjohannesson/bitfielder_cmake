#pragma once
#include <glm/glm.hpp>
#include "core/box2.h"
#include "gfx/core/texture_atlas.h"
#include "block/block_map_renderer.h"
#include "entity/entity_renderer.h"
#include "shadow_buffer.h"
#include "hud.h"

namespace bf {
    class WorldScene;

	class WorldRenderer {
    private:
        glm::mat4 shadowTransform, viewTransform, shadowViewTransform;
        Box2 screenBox;

        void loadTextureAtlas();

    public:
        static constexpr float SHADOW_OFFSET = 2.0f / 16.0f;

        glm::vec4 backgroundColor;

        TextureAtlas textureAtlas;
        Texture texture;

        SpriteProgram backSpriteProgram, shadowSpriteProgram, lightSpriteProgram;
        ShadowBuffer shadowBuffer;

        BlockMapRenderer map;
        EntityRenderer entities;

        HUD hud;

        inline Box2 getScreenBox() const { return screenBox; }

        void updateTransforms(const WorldScene &scene);
        void updateSize(glm::ivec2 size, WorldScene &scene);

        void render(WorldScene &scene);

        WorldRenderer(WorldScene &scene);
	};
}