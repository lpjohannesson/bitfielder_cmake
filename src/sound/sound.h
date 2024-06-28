#pragma once
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>

namespace bf {
    class Sound {
    private:
        static constexpr int CHANNEL_COUNT = 8;

        ALCdevice *device;
        ALCcontext *context;

        ALuint channels[CHANNEL_COUNT];
        int nextChannel = 0;

    public:
        void playSound(ALuint sound, bool loops, float volume = 1.0f, float pitch = 1.0f);

        Sound();
        ~Sound();
    };
}