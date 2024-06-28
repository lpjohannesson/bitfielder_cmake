#pragma once
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>

namespace bf {
    class SoundSet {
    public:
        std::vector<ALuint> sounds;

        ALuint loadSound(const char *path);
        void reset();

        ~SoundSet();
    };
}