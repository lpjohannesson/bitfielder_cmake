#pragma once
#include "engine/engine.h"
#include "gfx/sprite/sprite_renderer.h"
#include "client_input.h"

namespace bf {
	class Client {
	public:
        SpriteRenderer spriteRenderer;
		ClientInput clientInput;

        Client(Engine &engine);
	};

    extern Client *client;
}