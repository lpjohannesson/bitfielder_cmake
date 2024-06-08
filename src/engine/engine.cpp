#include "engine.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
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
}

void Engine::updateSize() {
	// Get window transform
	glm::vec2 halfSize = glm::floor(glm::vec2(windowSize) * 0.5f);
	windowTransform = glm::ortho(-halfSize.x, halfSize.x, halfSize.y, -halfSize.y);

	renderer.updateSize(windowSize);
	
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

		case SDL_KEYDOWN:
			input.keyDown(event.key.keysym.sym);
			break;
		
		case SDL_KEYUP:
			input.keyUp(event.key.keysym.sym);
			break;

		case SDL_CONTROLLERDEVICEADDED:
			if (gameController != nullptr) {
				SDL_GameControllerClose(gameController);
			}

			gameController = SDL_GameControllerOpen(event.cdevice.which);
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			input.joyButtonDown((SDL_GameControllerButton)event.cbutton.button);
			break;

		case SDL_CONTROLLERBUTTONUP:
			input.joyButtonUp((SDL_GameControllerButton)event.cbutton.button);
			break;

		case SDL_CONTROLLERAXISMOTION:
			input.joyAxisMotion((SDL_GameControllerAxis)event.caxis.axis, event.caxis.value);
			break;
		}
	}

	if (quitting) {
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

	// Render
	currentScene->render();
	SDL_GL_SwapWindow(window);

	return true;
}

Engine::Engine() : fullscreenAction(input) {
	engine = this;

	//SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

	windowSize = { 1280, 720 };

	// Create window
	window = SDL_CreateWindow(
		"bitfielder",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowSize.x, windowSize.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	glContext = SDL_GL_CreateContext(window);

	// Enable vsync
	SDL_GL_SetSwapInterval(1);

	// Use OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	glewExperimental = GL_TRUE;
	glewInit();

	updateSize();

	// Setup joysticks
	SDL_JoystickEventState(SDL_ENABLE);

	input.addKeyboardAction(fullscreenAction, SDLK_F11);
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