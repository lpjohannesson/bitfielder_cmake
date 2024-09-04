#pragma once
#include "render_system.h"
#include "gfx/sprite/sprite_batch.h"
#include "gfx/sprite/sprite_program.h"
#include "client/world/entity/components/sprite_component.h"
#include "client/client.h"

namespace bf {
	class PlayerRenderSystem : public RenderSystem {
    public:
        SpriteBatch skinSpriteBatch, overlaySpriteBatch;
        SpriteMesh mesh;
        SpriteProgram skinProgram;
        
        SpriteComponent sprite;

        void render(WorldScene &scene) override;

        PlayerRenderSystem();
	};
}