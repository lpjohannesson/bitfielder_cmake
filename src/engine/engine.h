#pragma once
#define SDL_MAIN_HANDLED
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

		float deltaTime = 0.0f;

		Scene *currentScene = nullptr;

		void endCurrentScene();

	public:
		inline glm::ivec2 getWindowSize() const { return windowSize; }
		inline float getDeltaTime() const { return deltaTime; }

		Renderer renderer;
		Input input;

		void changeScene(Scene *scene);

		bool update();

		Engine();
		~Engine();
	};

	extern Engine *engine;
}