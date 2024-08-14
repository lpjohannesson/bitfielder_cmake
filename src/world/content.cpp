#include "content.h"
#include "world.h"

using namespace bf;

Content::Content(World &world) : blocks(world), items(world), entities(world) {
    
}