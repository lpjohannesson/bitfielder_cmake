#include "engine.h"
#include <GL/glew.h>

using namespace bf;

Engine bf::engine;

void Engine::changeScene(Scene *scene) {
	// End old scene
	if (currentScene != nullptr) {
		currentScene->end();
		delete currentScene;
	}

	// Start new scene
	currentScene = scene;
	currentScene->start();
}

bool Engine::update() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			quitting = true;
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				glm::ivec2 windowSize = { event.window.data1, event.window.data2 };
				renderer.updateSize(windowSize);
			}
			break;
		}
	}

	if (quitting) {
		return false;
	}

	// Update scene
	currentScene->update();

	// Render
	currentScene->render();
	SDL_GL_SwapWindow(window);

	return true;
}

void Engine::start() {
	SDL_Init(SDL_INIT_VIDEO);

	constexpr glm::ivec2 windowSize = { 640, 480 };

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

	renderer.updateSize(windowSize);
}

void Engine::end() {
	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(glContext);

	SDL_Quit();
}