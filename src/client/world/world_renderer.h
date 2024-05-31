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

        SpriteProgram frontSpriteProgram, backSpriteProgram;

        BlockMapRenderer map;
        EntityRenderer entities;

        void updateSize(glm::ivec2 size);

        void render(const WorldScene &scene);

        inline WorldRenderer() :
            frontSpriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"),
            backSpriteProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment_back.glsl") {}
	};
}