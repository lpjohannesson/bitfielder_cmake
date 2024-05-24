#include "entities.h"

using namespace bf;

void Entities::addSystem(EntitySystem &system) {
    systems.push_back(&system);
}

void Entities::update(World &world) {
    // Update systems
    for (EntitySystem *system : systems) {
        system->update(world);
    }
}