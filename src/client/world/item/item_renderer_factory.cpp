#include "item_renderer_factory.h"
#include "client/scenes/world_scene.h"
#include <iostream>
#include "core/file_loader.h"
#include "components/item_renderer_component.h"
#include "world/registry/components/registry_name_component.h"

using namespace bf;

void ItemRendererFactory::createItem(entt::entity item, WorldScene &scene) {
    entt::registry &itemsRegistry = scene.world.items.registry;
    std::string name = itemsRegistry.get<RegistryNameComponent>(item).name;

    // Load json
    std::string jsonPath = "assets/renderers/blocks/" + name + ".json";
    rapidjson::Document document;
    
    if (!FileLoader::loadJson(jsonPath, document)) {
        std::cout << "Item renderer \"" << jsonPath << "\" could not be parsed." << std::endl;
        return;
    }

    rapidjson::Value &value = document["item"];

    
}

void ItemRendererFactory::start(WorldScene &scene) {
    for (entt::entity item : scene.world.items.entities) {
        createItem(item, scene);
    }
}

void ItemRendererFactory::end(WorldScene &scene) {
    entt::registry &itemsRegistry = scene.world.items.registry;

    for (entt::entity item : scene.world.items.entities) {
        if (!itemsRegistry.all_of<ItemRendererComponent>(item)) {
            continue;
        }

        delete itemsRegistry.get<ItemRendererComponent>(item).renderer;
        itemsRegistry.erase<ItemRendererComponent>(item);
    }
}