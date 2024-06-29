#include "sound.h"
#include <iostream>

using namespace bf;

void Sound::playSound(ALuint sound, bool loops, float volume, float pitch) {
    ALuint channel = channels[nextChannel];
    nextChannel = (nextChannel + 1) % CHANNEL_COUNT;

    alSourcei(channel, AL_BUFFER, sound);

    alSourcef(channel, AL_PITCH, pitch);
    alSourcef(channel, AL_GAIN, volume);
    alSource3f(channel, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(channel, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(channel, AL_LOOPING, loops);

    alSourcePlay(channel);
}

void Sound::reset() {
    for (int i = 0; i < CHANNEL_COUNT; i++) {
        alSourcei(channels[i], AL_BUFFER, NULL);
    }
}

Sound::Sound() {
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);

    for (int i = 0; i < CHANNEL_COUNT; i++) {
        alGenSources(1, &channels[i]);
    }
}

Sound::~Sound() {
    for (int i = 0; i < CHANNEL_COUNT; i++) {
        alDeleteSources(1, &channels[i]);
    }

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}