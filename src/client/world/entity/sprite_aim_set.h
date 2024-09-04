#pragma once
#include <string>
#include <glm/glm.hpp>
#include "gfx/core/texture_atlas.h"
#include "gfx/sprite/sprite_set.h"

namespace bf {
    class SpriteAimSet {
    private:
        void loadSprite(SpriteSet &sprites, const std::string path, glm::ivec2 counts, const TextureAtlas &textureAtlas);

    public:
        SpriteSet forward, up, down;

        SpriteSet &getFromAim(int aim);
        
        void loadSprites(const std::string path, glm::ivec2 counts, const TextureAtlas &textureAtlas);
    };
}