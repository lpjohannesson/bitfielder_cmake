#pragma once
#include <string>
#include <rapidjson/document.h>
#include "gfx/core/texture_atlas.h"

namespace bf {
    class WorldScene;

    class RendererFactory {
    public:
        static TextureSection getTexture(const std::string basePath, const rapidjson::Value &value, const WorldScene &scene);
    };
}