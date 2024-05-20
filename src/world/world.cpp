#include "world.h"

using namespace bf;

World::World() {
    content.loadContent(*this);
}