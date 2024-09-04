#pragma once
#include <glm/glm.hpp>
#include "core/box2.h"
#include "gfx/core/texture_atlas.h"
#include "block/block_map_renderer.h"
#include "shadow_buffer.h"
#include "hud.h"
#include "entity/systems/render/sprite_system.h"
#include "entity/systems/render/sprite_animator_system.h"
#include "entity/systems/render/player_render_system.h"

namespace bf {
    class WorldScene;

	class WorldRenderer {
    private:
        void loadTextureAtlas();

    public:
        static constexpr float SHADOW_OFFSET = 2.0f / 16.0f;

        glm::mat4 shadowTransform, viewTransform, shadowViewTransform;
        Box2 screenBox;

        glm::vec4 backgroundColor;

        TextureAtlas textureAtlas;
        Texture texture;

        SpriteProgram blockBackProgram, blockShadowProgram, blockLightProgram;
        ShadowBuffer shadowBuffer;

        BlockMapRenderer map;

        HUD hud;

        SpriteSystem spriteSystem;
        SpriteAnimatorSystem spriteAnimatorSystem;
        PlayerRenderSystem playerRenderSystem;

        void updateTransforms(const WorldScene &scene);
        void updateSize(glm::ivec2 size, WorldScene &scene);

        void render(WorldScene &scene);

        WorldRenderer();
	};
}