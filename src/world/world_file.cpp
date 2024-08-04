#include "world_file.h"
#include <filesystem>
#include <fstream>
#include "block/block_serializer.h"

using namespace bf;

void WorldFile::saveWorld(World &world, const std::string folderPath) {
    std::string blocksFolderPath = folderPath + "/blocks";

    std::filesystem::create_directories(folderPath);
    std::filesystem::create_directories(blocksFolderPath);

    for (auto &chunkPair : world.map.chunks) {
        BlockChunk &chunk = chunkPair.second;

        Packet packet;
        BlockSerializer::writeChunk(chunk, world, packet);

        // Save chunk with index as name
        std::string chunkPath = blocksFolderPath + "/" + std::to_string(chunk.getMapIndex()) + ".dat";

        std::ofstream chunkFile(chunkPath, std::ios::binary);
        chunkFile.write((char*)packet.data.data(), packet.data.size());
    }
}