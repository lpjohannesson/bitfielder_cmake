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
	private:
		glm::mat4 menuTransform, menuInverseTransform;

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

		static void renderLogo(TextureSection texture, SpriteMesh &mesh);

		glm::mat4 getMenuTransform() const { return menuTransform; }
		glm::mat4 getMenuInverseTransform() const { return menuInverseTransform; }

		float getRandomPitch();

		void updateSize(glm::ivec2 size);

        Client(Engine &engine);
	};

    extern Client *client;
}