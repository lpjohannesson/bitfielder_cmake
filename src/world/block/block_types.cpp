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

int BlockTypes::getBlockByName(const std::string name) const {
    auto foundBlock = blockNames.find(name);

    if (foundBlock == blockNames.end()) {
        return 0;
    }

    return foundBlock->second;
}

entt::entity BlockTypes::createBlock(const std::string name) {
    entt::entity block = registry.create();

    registry.emplace<BlockNameComponent>(block, BlockNameComponent { name });

    int blockIndex = (int)blocks.size();
    blocks.push_back(block);

    blockNames.emplace(name, blockIndex);

    return block;
}