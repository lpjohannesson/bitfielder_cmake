#pragma once
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include <glm/glm.hpp>

namespace bf {
    class Sound {
    public:
        ALuint sound;
        bool loops = false;
        float volume = 1.0f;
        float pitch = 1.0f;
    };

    class SoundPlayer {
    private:
        static constexpr int CHANNEL_COUNT = 8;

        ALCdevice *device;
        ALCcontext *context;

        ALuint channels[CHANNEL_COUNT];
        int nextChannel = 0;

    public:
        void playSound(const Sound &sound);
        void reset();

        SoundPlayer();
        ~SoundPlayer();
    };
}