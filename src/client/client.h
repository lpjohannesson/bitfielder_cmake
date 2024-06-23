#pragma once
#include "engine/engine.h"
#include "gfx/sprite/sprite_renderer.h"
#include "gfx/sprite/sprite_batch.h"
#include "gfx/text/font.h"
#include "gfx/core/texture.h"
#include "client_input.h"

namespace bf {
	class Client {
	public:
		ClientInput clientInput;

        SpriteRenderer spriteRenderer;
		SpriteProgram spriteProgram;
		
		TextureAtlas fontTextureAtlas;
		Texture fontTexture;
		SpriteBatch fontSpriteBatch;

		Font font;

		static glm::mat4 getMenuTransform();
		static void renderLogo(TextureSection texture, SpriteMesh &mesh);

        Client(Engine &engine);
	};

    extern Client *client;
}