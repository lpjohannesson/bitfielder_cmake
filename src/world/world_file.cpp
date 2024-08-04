#include "world_file.h"
#include <filesystem>
#include <fstream>
#include "core/file_loader.h"
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

bool WorldFile::loadWorld(World &world, const std::string folderPath) {
    if (!std::filesystem::is_directory(folderPath)) {
        return false;
    }

    // Load blocks
    std::string blocksFolderPath = folderPath + "/blocks";
    
    std::vector<std::filesystem::path> blockFilePaths;
    FileLoader::getFilePathObjects(blocksFolderPath, blockFilePaths);

    for (const auto &filePath : blockFilePaths) {
        int chunkIndex = std::stoi(filePath.filename());
        BlockChunk &chunk = world.map.createChunk(chunkIndex);

        std::ifstream chunkFile(filePath.string(), std::ios::binary);

        Packet packet;
        packet.data = { std::istreambuf_iterator<char>(chunkFile), {} };

        BlockSerializer::readChunk(packet, chunk, world);
    }

    return true;
}