#pragma once
#include <SDL2/SDL.h>

namespace bf {
	class GameTime {
    private:
        float deltaTime = 0.0f;
        Uint32 lastTime = 0;

    public:
        inline float getDeltaTime() const { return deltaTime; }

        void update();
        void reset();
    };

    extern GameTime gameTime;
}