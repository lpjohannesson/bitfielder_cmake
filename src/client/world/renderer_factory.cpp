#include "renderer_factory.h"
#include "client/scenes/world_scene.h"

using namespace bf;

TextureSection RendererFactory::getTexture(const std::string basePath, const rapidjson::Value &value, const WorldScene &scene) {
    std::string texturePath = value["texture"].GetString();
    std::string textureFullPath = basePath + texturePath + ".png";

    const TextureAtlas &textureAtlas = scene.worldRenderer.textureAtlas;

    return textureAtlas.getSection(textureFullPath);
}