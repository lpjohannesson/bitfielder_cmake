#include "font_factory.h"
#include <iostream>
#include "core/file_loader.h"

using namespace bf;

void FontFactory::loadFont(std::string name, const TextureAtlas &textureAtlas, Font &font) {
    // Load json
    std::string jsonPath = "assets/fonts/" + name + ".json";
    rapidjson::Document document;
    
    if (!FileLoader::loadJson(jsonPath, document)) {
        std::cout << "Font \"" << jsonPath << "\" could not be parsed." << std::endl;
        return;
    }

    std::string texturePath = "assets/textures/fonts/" + std::string(document["texture"].GetString()) + ".png";
    font.loadFont(textureAtlas.getSection(texturePath));

    font.lineHeight = document["lineHeight"].GetFloat();

    auto spacings = document["spacings"].GetArray();

    for (int i = 0; i < (int)spacings.Size(); i++) {
        font.spacings[i] = spacings[i].GetInt();
    }
}