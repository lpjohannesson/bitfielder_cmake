#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "gfx/renderer.h"
#include "scene.h"

namespace bf {
	class Engine {
	private:
		bool quitting = false;

		SDL_Window *window;
		SDL_GLContext glContext;

		Scene *currentScene;

	public:
		Renderer renderer;

		void changeScene(Scene *scene);

		bool update();

		void start();
		void end();
	};

	extern Engine engine;
}