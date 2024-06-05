#include "block_types.h"
#include "components/block_name_component.h"

using namespace bf;

entt::entity BlockTypes::getBlock(int index) const {
    // Default if out of range
    if (index < 0 || index >= blocks.size()) {
        return blocks.at(0);
    }

    return blocks.at(index);
}

entt::entity BlockTypes::createBlock(std::string name) {
    entt::entity block = registry.create();

    registry.emplace<BlockNameComponent>(block, BlockNameComponent { name });

    blocks.push_back(block);

    return block;
}