#include "sound.h"
#include <iostream>

using namespace bf;

void SoundPlayer::playSound(const Sound &sound) {
    ALuint channel = channels[nextChannel];
    nextChannel = (nextChannel + 1) % CHANNEL_COUNT;

    alSourceStop(channel);

    alSourcei(channel, AL_BUFFER, sound.sound);
    
    alSource3f(channel, AL_POSITION, 0.0f, 0.0f, 0.0f );
    alSource3f(channel, AL_VELOCITY, 0.0f, 0.0f, 0.0f );

    alSourcef(channel, AL_PITCH, sound.pitch);
    alSourcef(channel, AL_GAIN, sound.volume);
    alSourcei(channel, AL_LOOPING, sound.loops);

    alSourcePlay(channel);
}

void SoundPlayer::reset() {
    for (int i = 0; i < CHANNEL_COUNT; i++) {
        alSourcei(channels[i], AL_BUFFER, 0);
    }
}

SoundPlayer::SoundPlayer() {
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);

    for (int i = 0; i < CHANNEL_COUNT; i++) {
        alGenSources(1, &channels[i]);
    }
}

SoundPlayer::~SoundPlayer() {
    for (int i = 0; i < CHANNEL_COUNT; i++) {
        alDeleteSources(1, &channels[i]);
    }

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}