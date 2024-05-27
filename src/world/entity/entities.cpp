#include "entities.h"
#include "components/id_component.h"

using namespace bf;

bool Entities::getEntity(int id, entt::entity &entity) {
    auto foundEntity = entityIDs.find(id);

    if (foundEntity == entityIDs.end()) {
        return false;
    }

    entity = foundEntity->second;
    return true;
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
    entt::entity entity;

    if (!getEntity(id, entity)) {
        return;
    }

    registry.destroy(entity);
    entityIDs.erase(id);
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