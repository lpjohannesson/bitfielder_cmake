#pragma once
#include <string>
#include <entt/entt.hpp>
#include <unordered_map>
#include "sound/sound_set.h"

namespace bf {
    class WorldScene;

    class BlockSounds {
    private:
        std::unordered_map<std::string, SoundSet> soundSets; 

    public:
        static void playBlockSound(WorldScene &scene, entt::entity block, float volume = 0.75f);

        SoundSet *getSoundSet(std::string name);
        void loadSounds();
    };
}