#include "inventory.h"
#include "world/world.h"
#include "world/registry/components/registry_index_component.h"

using namespace bf;

void Inventory::setItem(int slot, entt::entity item, World &world) {
    // Set item index, add 1 for null as zero
    int itemIndex = world.items.registry.get<RegistryIndexComponent>(item).index;

    items[slot].itemIndex = itemIndex + 1;
}

bool Inventory::getItem(int slot, World &world, entt::entity &result) {
    int itemIndex = items[slot].itemIndex - 1;

    if (itemIndex < 0) {
        return false;
    }

    result = world.items.getEntity(itemIndex);
    return true;
}

bool Inventory::getSelectedItem(World &world, entt::entity &result) {
    return getItem(selectedIndex, world, result);
}