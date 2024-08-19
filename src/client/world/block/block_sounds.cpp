#include "block_sounds.h"
#include "client/scenes/world_scene.h"
#include "core/file_loader.h"
#include "client/world/block/components/block_sound_component.h"

using namespace bf;

void BlockSounds::playBlockSound(WorldScene &scene, entt::entity block, float volume) {
    entt::registry &blocksRegistry = scene.world.blocks.registry;

    if (!blocksRegistry.all_of<BlockSoundComponent>(block)) {
        return;
    }

    BlockSoundComponent &blockSound = blocksRegistry.get<BlockSoundComponent>(block);

    if (blockSound.soundSet == nullptr) {
        return;
    }

    // Play random sound
    int randomIndex = client->randomInt(client->randomEngine) % (int)blockSound.soundSet->sounds.size();
    
    Sound &sound = blockSound.soundSet->sounds[randomIndex];
    sound.volume = volume;

    client->playRandomPitchSound(sound);
}

SoundSet *BlockSounds::getSoundSet(std::string name) {
    auto foundSet = soundSets.find(name);

    if (foundSet == soundSets.end()) {
        return nullptr;
    }
    
    return &soundSets.at(name);
}

void BlockSounds::loadSounds() {
    const std::string basePath = "assets/sounds/blocks/";

    // Get all folders
    std::vector<std::string> directoryNames;
    FileLoader::getDirectoryNames(basePath.c_str(), directoryNames);

    for (const std::string &directoryName : directoryNames) {
        // Attach folder name to sound set
        SoundSet &soundSet = soundSets.emplace(
            std::piecewise_construct, std::make_tuple(directoryName), std::make_tuple()).first->second;

        // Load all sounds in folder
        std::string folderPath = basePath + directoryName;

        std::vector<std::string> filePaths;
        FileLoader::getFilePaths(folderPath.c_str(), filePaths);

        for (const std::string &filePath : filePaths) {
            soundSet.createSound(filePath.c_str());
        }
    }
}