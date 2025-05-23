#include "game_time.h"

using namespace bf;

GameTime bf::gameTime;

void GameTime::reset() {
	lastTime = SDL_GetTicks();
}

void GameTime::update() {
	Uint32 currentTime = SDL_GetTicks();

	deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;
}