#include "font_factory.h"
#include <iostream>
#include <rapidjson/document.h>
#include "core/file_loader.h"

using namespace bf;

void FontFactory::loadFont(std::string name, const TextureAtlas &textureAtlas, Font &font) {
    // Load json
    std::string jsonPath = "assets/fonts/" + name + ".json";
    std::string json;
    
    if (!FileLoader::loadText(jsonPath.c_str(), json)) {
        return;
    }

    // Parse json
    rapidjson::Document document;
    
    if (document.Parse(json.c_str()).HasParseError()) {
        std::cout << "Font \"" << jsonPath << "\" could not be parsed." << std::endl;
        return;
    }

    std::string texturePath = "assets/textures/fonts/" + std::string(document["texture"].GetString()) + ".png";
    font.loadFont(textureAtlas.getSection(texturePath));

    auto spacings = document["spacings"].GetArray();

    for (int i = 0; i < spacings.Size(); i++) {
        font.spacings[i] = spacings[i].GetInt();
    }
}