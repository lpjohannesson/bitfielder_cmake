#pragma once
#include "block/block_content.h"
#include "item/item_content.h"
#include "entity/entity_content.h"

namespace bf {
    class World;

    class Content {
    public:
        BlockContent blocks;
        ItemContent items;
        EntityContent entities;

        Content(World &world);
    };
}