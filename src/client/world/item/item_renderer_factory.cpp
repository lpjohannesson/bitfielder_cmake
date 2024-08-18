#include "item_renderer_factory.h"
#include "client/scenes/world_scene.h"
#include <iostream>
#include "core/file_loader.h"
#include "components/item_texture_component.h"
#include "world/registry/components/registry_name_component.h"

using namespace bf;

TextureSection ItemRendererFactory::getItemTexture(const rapidjson::Value &value, const WorldScene &scene) {
    return RendererFactory::getTexture("assets/textures/world/items/", value, scene);
}

void ItemRendererFactory::createItem(entt::entity item, WorldScene &scene) {
    entt::registry &itemsRegistry = scene.world.items.registry;
    std::string name = itemsRegistry.get<RegistryNameComponent>(item).name;

    // Load json
    std::string jsonPath = "assets/renderers/items/" + name + ".json";
    rapidjson::Document document;
    
    if (!FileLoader::loadJson(jsonPath, document)) {
        std::cout << "Item renderer \"" << jsonPath << "\" could not be parsed." << std::endl;
        return;
    }

    rapidjson::Value &value = document["item"];

    itemsRegistry.emplace<ItemTextureComponent>(item,
        ItemTextureComponent { getItemTexture(value, scene).uvBox });
}

void ItemRendererFactory::start(WorldScene &scene) {
    for (entt::entity item : scene.world.items.entities) {
        createItem(item, scene);
    }
}

void ItemRendererFactory::end(WorldScene &scene) {

}