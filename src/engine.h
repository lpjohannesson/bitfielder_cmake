#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "gfx/core/renderer.h"
#include "scene.h"

namespace bf {
	class Engine {
	private:
		bool quitting = false;

		SDL_Window *window;
		SDL_GLContext glContext;

		Scene *currentScene = nullptr;

		glm::ivec2 windowSize;

		void endCurrentScene();

	public:
		inline glm::ivec2 getWindowSize() const { return windowSize; }

		Renderer renderer;

		void changeScene(Scene *scene);

		bool update();

		Engine();
		~Engine();
	};

	extern Engine *engine;
}