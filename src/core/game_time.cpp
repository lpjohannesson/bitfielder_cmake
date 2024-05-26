#include "game_time.h"

using namespace bf;

GameTime bf::gameTime;

void GameTime::update() {
    static Uint32 lastTime = 0;
	Uint32 currentTime = SDL_GetTicks();

	deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;
}