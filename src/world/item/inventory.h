#pragma once
#include <array>

namespace bf {
    class ItemSlot {
    public:
        int itemIndex;
    };

    class Inventory {
    public:
        std::array<ItemSlot, 10> itemSlots;
    };
}