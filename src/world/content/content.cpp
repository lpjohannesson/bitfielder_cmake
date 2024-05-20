#include "content.h"
#include "world/world.h"

using namespace bf;

void Content::loadContent(World &world) {
    airBlock = world.blocks.createBlock();
    testBlock = world.blocks.createBlock();
}