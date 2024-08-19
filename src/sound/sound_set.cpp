#include "sound_set.h"
#include <iostream>
#include <SDL2/SDL_audio.h>

using namespace bf;

Sound &SoundSet::createSound(const char *path) {
    sounds.emplace_back();
    Sound &sound = sounds.back();

    alGenBuffers(1, &sound.sound);

    SDL_AudioSpec wavSpec;
    Uint8 *wavBuffer;
    Uint32 wavLength;

    if (SDL_LoadWAV(path, &wavSpec, &wavBuffer, &wavLength) == nullptr) {
        std::cout << "Sound \"" << path << "\" could not be found." << std::endl;
        return sound;
    }

    alBufferData(sound.sound, AL_FORMAT_STEREO16, wavBuffer, wavLength, wavSpec.freq);

    SDL_FreeWAV(wavBuffer);

    return sound;
}

void SoundSet::reset() {
    for (Sound &sound : sounds) {
        alDeleteBuffers(1, &sound.sound);
    }

    sounds.clear();
}

SoundSet::~SoundSet() {
    reset();
}