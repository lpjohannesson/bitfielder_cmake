#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "gfx/core/renderer.h"
#include "scene.h"
#include "input.h"

namespace bf {
	class Engine {
	private:
		bool quitting = false;
		glm::ivec2 windowSize;

		SDL_Window *window;
		SDL_GLContext glContext;

		SDL_GameController *gameController = nullptr;

		Scene *currentScene = nullptr;

		bool fullscreen = false;

		void endCurrentScene();

	public:
		inline glm::ivec2 getWindowSize() const { return windowSize; }

		Renderer renderer;
		
		Input input;
		InputAction fullscreenAction;

		void changeScene(Scene *scene);

		bool update();

		Engine();
		~Engine();
	};

	extern Engine *engine;
}