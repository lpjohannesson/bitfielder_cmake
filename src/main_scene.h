#pragma once
#include "scene.h"
#include "gfx/sprite_renderer.h"
#include "gfx/sprite_mesh.h"
#include "gfx/sprite_batch.h"
#include "gfx/texture_atlas.h"

namespace bf {
	class MainScene : public Scene {
	public:
		SpriteRenderer spriteRenderer;
		SpriteMesh spriteMesh;

		TextureAtlas textureAtlas;

		SpriteBatch spriteBatch;

		void update() override;
		void render() override;

		void start() override;
		void end() override;
	};
}