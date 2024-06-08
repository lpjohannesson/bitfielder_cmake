#pragma once
#include <string>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "gfx/sprite/animation/sprite_frames.h"
#include "gfx/sprite/sprite_batch.h"
#include "gfx/core/texture_atlas.h"

namespace bf {
	class Font {
	private:
		static constexpr glm::ivec2 FRAME_COUNTS = { 16, 6 };
		
		SpriteFrames frames;
		glm::ivec2 frameSize;

		int characterWidths[FRAME_COUNTS.x * FRAME_COUNTS.y];

		static int getCharacterIndex(char c);

		int createCharacterWidth(SDL_Surface *surface, glm::ivec2 start);

	public:
		void drawText(std::string text, SpriteBatch &spriteBatch, glm::vec2 position);

		void loadFont(SDL_Surface *surface, TextureSection section, int spaceWidth);
	};
}