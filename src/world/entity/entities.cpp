#include "entities.h"
#include "components/id_component.h"

using namespace bf;

entt::entity *Entities::getEntity(int id) {
    auto foundEntity = entityIDs.find(id);

    if (foundEntity == entityIDs.end()) {
        return nullptr;
    }

    return &foundEntity->second;
}

entt::entity Entities::spawnEntity(int id) {
    entt::entity entity = registry.create();
    entityIDs.emplace(id, entity);

    // Add ID component
    registry.emplace<IDComponent>(entity, IDComponent { id });

    return entity;
}

entt::entity Entities::spawnEntity() {
    // Spawn entity with new ID
    entt::entity entity = spawnEntity(nextID);
    nextID++;

    return entity;
}

void Entities::despawnEntity(int id) {
    entt::entity *entity = getEntity(id);

    if (entity == nullptr) {
        return;
    }

    entityIDs.erase(id);
    registry.destroy(*entity);
}

void Entities::addSystem(EntitySystem &system) {
    systems.push_back(&system);
}

void Entities::update(World &world) {
    // Update systems
    for (EntitySystem *system : systems) {
        system->update(world);
    }
}