#pragma once
#include <string>
#include <glm/glm.hpp>
#include "gfx/sprite/sprite_frames.h"
#include "gfx/sprite/sprite_batch.h"
#include "gfx/core/texture_atlas.h"

namespace bf {
	class FontProperties {
	public:
		glm::vec2 position{};
		bool centered = false;
		glm::vec4 color{ 1.0f };
	};

	class Font {
	private:
		static constexpr glm::ivec2 FRAME_COUNTS = { 16, 6 };

		SpriteFrames frames;
		glm::ivec2 frameSize;

		static int getCharacterIndex(char c);

	public:
		int spacings[FRAME_COUNTS.x * FRAME_COUNTS.y];
		float lineHeight = 0.0f;

		float getTextLength(std::string text, int end) const;
		glm::vec2 getRenderPosition(std::string text, FontProperties &properties) const;

		void drawText(std::string text, FontProperties &properties, SpriteBatch &spriteBatch);

		void loadFont(TextureSection section);
	};
}