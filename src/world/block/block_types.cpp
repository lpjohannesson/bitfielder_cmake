#include "block_types.h"

using namespace bf;

entt::entity BlockTypes::getBlock(int index) const {
    return blocks.at(index);
}

entt::entity BlockTypes::createBlock() {
    // Push block to vector at index
    int index = blocks.size();

    entt::entity block = registry.create();
    blocks.push_back(block);

    return block;
}