#pragma once
#include <entt/entt.hpp>
#include "render_system.h"
#include "gfx/sprite/sprite_batch.h"
#include "client/world/entity/components/sprite_component.h"
#include "world/entity/components/position_component.h"
#include "client/client.h"

namespace bf {
	class SpriteSystem : public RenderSystem {
    public:
        SpriteBatch spriteBatch;
        SpriteMesh mesh;

        static void drawSprite(entt::entity entity, const SpriteComponent &sprite, const PositionComponent position, WorldScene &scene, Sprite &result);

        void render(WorldScene &scene) override;

        SpriteSystem();
	};
}