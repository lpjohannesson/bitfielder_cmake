#include "block_serializer.h"
#include <vector>
#include <unordered_map>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "world/registry/components/registry_name_component.h"

using namespace bf;

void BlockSerializer::createPaletteIndex(int blockIndex, World &world, std::vector<std::string> &paletteList, std::unordered_map<int, uint16_t> &paletteMap) {
    auto foundEntry = paletteMap.find(blockIndex);

    // Skip if already exists
    if (foundEntry != paletteMap.end()) {
        return;
    }

    // Map new palette entry to block index
    uint16_t paletteIndex = (uint16_t)paletteList.size();
    paletteMap.emplace(blockIndex, paletteIndex);

    // Add block name
    entt::entity block = world.blocks.getEntity(blockIndex);
    std::string blockName = world.blocks.registry.get<RegistryNameComponent>(block).name;

    paletteList.push_back(blockName);
}

void BlockSerializer::writeChunk(BlockChunk &chunk, World &world, Packet &result) {
    Packet packet;

    // Create block palette
    std::vector<std::string> paletteList;
    std::unordered_map<int, uint16_t> paletteMap;

    for (int y = 0; y < BlockChunk::SIZE.y; y++) {
        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            glm::ivec2 position = { x, y };
            BlockData &blockData = chunk.getBlock(position);

            // Get palette indices
            createPaletteIndex(blockData.getFrontIndex(), world, paletteList, paletteMap);
            createPaletteIndex(blockData.getBackIndex(), world, paletteList, paletteMap);
        }
    }
    
    // Create metadata
    rapidjson::Document document;
    document.SetObject();

    // Serialize palette
    rapidjson::Value paletteListValue(rapidjson::kArrayType);

    for (const std::string paletteIndex : paletteList) {
        rapidjson::Value paletteValue;
        paletteValue.SetString(paletteIndex.c_str(), document.GetAllocator());

        paletteListValue.PushBack(paletteValue, document.GetAllocator());
    }

    document.AddMember("palette", paletteListValue, document.GetAllocator());

    // Write metadata
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    document.Accept(writer);
    packet << std::string(buffer.GetString(), buffer.GetSize());

    // Write block data
    for (int y = 0; y < BlockChunk::SIZE.y; y++) {
        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            glm::ivec2 position = { x, y };
            BlockData &blockData = chunk.getBlock(position);

            packet <<
                paletteMap.at(blockData.getFrontIndex()) <<
                paletteMap.at(blockData.getBackIndex()) <<
                blockData.getLightData();
        }
    }

    // Compress to result
    packet.compressPacket(result);
}

void BlockSerializer::readChunk(Packet &input, BlockChunk &chunk, World &world) {
    Packet packet;
    input.decompressPacket(64 * 1024, packet);

    // Read metadata
    std::string metadataText;
    packet >> metadataText;

    rapidjson::Document document;
    document.Parse(metadataText.c_str());

    // Read block palette
    std::vector<int> paletteList;

    for (auto &paletteEntry : document["palette"].GetArray()) {
        int blockIndex = world.blocks.getEntityByName(paletteEntry.GetString());
        paletteList.push_back(blockIndex);
    }

    for (int y = 0; y < BlockChunk::SIZE.y; y++) {
        for (int x = 0; x < BlockChunk::SIZE.x; x++) {
            uint16_t frontPaletteIndex, backPaletteIndex;
            packet >> frontPaletteIndex >> backPaletteIndex;
            
            glm::ivec2 position = { x, y };
            BlockData &blockData = chunk.getBlock(position);

            blockData.setFrontIndex(paletteList.at(frontPaletteIndex));
            blockData.setBackIndex(paletteList.at(backPaletteIndex));

            packet >> blockData.getLightData();
        }
    }
}