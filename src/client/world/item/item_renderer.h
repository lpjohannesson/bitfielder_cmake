#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <rapidjson/document.h>
#include "gfx/sprite/sprite_batch.h"

namespace bf {
    class WorldScene;

    class ItemRenderData {
    public:
        WorldScene &scene;
        SpriteBatch &spriteBatch;

        glm::vec2 position;
    };

    class ItemRenderer {
    public:
        inline virtual void render(const ItemRenderData &renderData) {}
    };
}