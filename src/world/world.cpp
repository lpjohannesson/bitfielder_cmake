#include "world.h"

using namespace bf;

void World::update() {
    entities.update(*this);
}