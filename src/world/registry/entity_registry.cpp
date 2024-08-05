#include "entity_registry.h"
#include "components/registry_index_component.h"
#include "components/registry_name_component.h"

using namespace bf;

entt::entity EntityRegistry::getEntity(int index) const {
    // Default if out of range
    if (index < 0 || index >= entities.size()) {
        return entities.at(0);
    }

    return entities.at(index);
}

int EntityRegistry::getEntityByName(const std::string name) const {
    auto foundEntity = entityNames.find(name);

    if (foundEntity == entityNames.end()) {
        return 0;
    }

    return foundEntity->second;
}

entt::entity EntityRegistry::createEntity(const std::string name) {
    entt::entity entity = registry.create();

    int index = (int)entities.size();
    entities.push_back(entity);

    entityNames.emplace(name, index);

    registry.emplace<RegistryIndexComponent>(entity, RegistryIndexComponent { index });
    registry.emplace<RegistryNameComponent>(entity, RegistryNameComponent { name });

    return entity;
}