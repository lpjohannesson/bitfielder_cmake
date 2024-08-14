#include "item_content.h"
#include "world/world.h"

using namespace bf;

ItemContent::ItemContent(World &world) {
    dirt = world.items.createEntity("dirt");
    stone = world.items.createEntity("stone");
}