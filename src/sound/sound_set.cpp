#include "sound_set.h"
#include <iostream>
#include <SDL2/SDL_audio.h>

using namespace bf;

ALuint SoundSet::loadSound(const char *path) {
    ALuint sound;
    alGenBuffers(1, &sound);

    SDL_AudioSpec wavSpec;
    Uint8 *wavBuffer;
    Uint32 wavLength;

    if (SDL_LoadWAV(path, &wavSpec, &wavBuffer, &wavLength) == nullptr) {
        std::cout << "Sound \"" << path << "\" could not be found." << std::endl;
        return sound;
    }

    alBufferData(sound, AL_FORMAT_STEREO16, wavBuffer, wavLength, wavSpec.freq);

    SDL_FreeWAV(wavBuffer);

    sounds.push_back(sound);

    return sound;
}

void SoundSet::reset() {
    for (ALuint sound : sounds) {
        alDeleteBuffers(1, &sound);
    }

    sounds.clear();
}

SoundSet::~SoundSet() {
    reset();
}