#pragma once
#include <random>
#include "engine/engine.h"
#include "gfx/sprite/sprite_renderer.h"
#include "gfx/sprite/sprite_batch.h"
#include "gfx/text/font.h"
#include "gfx/core/texture.h"
#include "client_input.h"

namespace bf {
	class Client {
	public:
		std::default_random_engine randomEngine;
        std::uniform_int_distribution<int> randomInt;
        std::uniform_real_distribution<float> randomFloat;

		ClientInput clientInput;

        SpriteRenderer spriteRenderer;
		SpriteProgram spriteProgram;
		
		TextureAtlas fontTextureAtlas;
		Texture fontTexture;
		SpriteBatch fontSpriteBatch;

		Font font;

		float maxRandomPitch;

		float getRandomPitch();

		static glm::mat4 getMenuTransform();
		static void renderLogo(TextureSection texture, SpriteMesh &mesh);

        Client(Engine &engine);
	};

    extern Client *client;
}