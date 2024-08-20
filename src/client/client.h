#pragma once
#include "engine/engine.h"
#include "gfx/sprite/sprite_renderer.h"
#include "gfx/sprite/sprite_batch.h"
#include "gfx/text/font.h"
#include "gfx/core/texture.h"
#include "sound/sound.h"
#include "client_input.h"
#include "core/random.h"

namespace bf {
	class Client {
	private:
		glm::mat4 menuTransform, menuInverseTransform;

	public:
		Random random;
		
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
		void playRandomPitchSound(Sound &sound);

		void updateSize(glm::ivec2 size);

        Client(Engine &engine);
	};

    extern Client *client;
}