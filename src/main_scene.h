#pragma once
#include "engine/scene.h"
#include "world/world.h"
#include "client/world/world_renderer.h"
#include "client/world/content/client_content.h"

namespace bf {
	class MainScene : public Scene {
	public:
		World world;
		WorldRenderer worldRenderer;

		ClientContent clientContent;

		void updateSize(glm::ivec2 size) override;

		void update() override;
		void render() override;

		void start() override;
		void end() override;
	};
}