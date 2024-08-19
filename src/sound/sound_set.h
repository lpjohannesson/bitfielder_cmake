#pragma once
#include <vector>
#include "sound.h"

namespace bf {
    class SoundSet {
    public:
        std::vector<Sound> sounds;

        Sound &createSound(const char *path);
        void reset();

        ~SoundSet();
    };
}