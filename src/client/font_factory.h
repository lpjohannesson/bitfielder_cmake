#pragma once
#include <string>
#include "gfx/text/font.h"

namespace bf {
    class FontFactory {
    public:
        static void loadFont(std::string name, const TextureAtlas &textureAtlas, Font &font);
    };
}