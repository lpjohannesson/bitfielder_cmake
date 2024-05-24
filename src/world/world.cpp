#include "world.h"

using namespace bf;

void World::update() {
    entities.update(*this);
}

World::World() {
    content.loadContent(*this);
}