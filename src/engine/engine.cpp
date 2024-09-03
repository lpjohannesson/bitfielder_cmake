#include "engine.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "gfx/core/renderer.h"
#include "core/game_time.h"

using namespace bf;

Engine *bf::engine;

void Engine::endCurrentScene() {
	if (currentScene != nullptr) {
		currentScene->end();
		delete currentScene;
	}
}

void Engine::changeScene(Scene *scene) {
	endCurrentScene();

	// Start new scene
	currentScene = scene;
	currentScene->start();
	currentScene->updateSize(windowSize);

	changingScene = true;
}

void Engine::updateSize() {
	// Get window transform
	glm::vec2 halfSize = glm::floor(glm::vec2(windowSize) * 0.5f);
	windowTransform = glm::ortho(-halfSize.x, halfSize.x, halfSize.y, -halfSize.y);

	Renderer::updateSize(windowSize);
	
	if (currentScene != nullptr) {
		currentScene->updateSize(windowSize);
	}
}

bool Engine::update() {
	gameTime.update();

	// Window events
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			quitting = true;
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				windowSize = { event.window.data1, event.window.data2 };
				updateSize();
			}
			break;

		case SDL_CONTROLLERDEVICEADDED:
			if (gameController != nullptr) {
				SDL_GameControllerClose(gameController);
			}

			gameController = SDL_GameControllerOpen(event.cdevice.which);
			break;
		}

		input.processEvent(event);
	}

	if (quitting) {
		sound.reset();

		return false;
	}

	// Toggle fullscreen
	if (fullscreenAction.justPressed()) {
		fullscreen = !fullscreen;
		SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	}

	// Update scene
	currentScene->update();

	// Update input for previous state
	input.update();

	// Check end
	if (quitting) {
		return false;
	}

	if (changingScene) {
		changingScene = false;
		return true;
	}

	// Render
	currentScene->render();
	SDL_GL_SwapWindow(window);

	return true;
}

Engine::Engine() : fullscreenAction(input) {
	engine = this;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);

#ifdef NX
	windowSize = { 1280, 720 };
#else
	windowSize = { 640, 480 };
#endif

	window = SDL_CreateWindow(
		"bitfielder",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowSize.x, windowSize.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	glContext = SDL_GL_CreateContext(window);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	SDL_GL_SetSwapInterval(1);

	updateSize();

	// Setup joysticks
	SDL_JoystickEventState(SDL_ENABLE);

	if (SDL_IsGameController(0)) {
		gameController = SDL_GameControllerOpen(0);
	}

	SDL_JoystickOpen(0);

	input.keyboard.addAction(fullscreenAction, SDLK_F11);
}

Engine::~Engine() {
	endCurrentScene();

	if (gameController != nullptr) {
		SDL_GameControllerClose(gameController);
	}

	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(glContext);

	SDL_Quit();
}