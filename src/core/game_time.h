#pragma once
#include <SDL2/SDL.h>

namespace bf {
	class GameTime {
    private:
        float deltaTime = 0.0f;

    public:
        inline float getDeltaTime() const { return deltaTime; }

        void update();
    };

    extern GameTime gameTime;
}