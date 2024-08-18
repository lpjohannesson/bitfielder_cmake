#pragma once
#include <array>
#include <entt/entt.hpp>

namespace bf {
    class World;

    class ItemSlot {
    public:
        uint16_t itemIndex;
    };

    class Inventory {
    public:
        std::array<ItemSlot, 10> items;
        int selectedIndex = 0;

        void setItem(int slot, entt::entity item, World &world);

        bool getItem(int slot, World &world, entt::entity &result);
        bool getSelectedItem(World &world, entt::entity &result);
    };
}