#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "gfx/core/renderer.h"
#include "scene.h"
#include "input.h"
#include "sound/sound.h"

namespace bf {
	class Engine {
	private:
		glm::ivec2 windowSize;
		glm::mat4 windowTransform;

		SDL_Window *window;
		SDL_GLContext glContext;

		SDL_GameController *gameController = nullptr;

		Scene *currentScene = nullptr;

		bool fullscreen = false;

		void endCurrentScene();

		void updateSize();

	public:
		inline glm::ivec2 getWindowSize() const { return windowSize; }
		inline glm::mat4 getWindowTransform() const { return windowTransform; }

		bool quitting = false;

		Renderer renderer;
		Sound sound;
		
		Input input;
		InputAction fullscreenAction;

		void changeScene(Scene *scene);

		bool update();

		Engine();
		~Engine();
	};

	extern Engine *engine;
}