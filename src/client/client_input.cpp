#include "client_input.h"
#include "engine/engine.h"

using namespace bf;

ClientInput::ClientInput() {
    Input &input = engine->input;

    input.addKeyboardAction(up, SDLK_UP);
    input.addKeyboardAction(down, SDLK_DOWN);
    input.addKeyboardAction(left, SDLK_LEFT);
    input.addKeyboardAction(right, SDLK_RIGHT);
    input.addKeyboardAction(jump, SDLK_z);
    input.addKeyboardAction(modifyBlockFront, SDLK_x);
    input.addKeyboardAction(modifyBlockBack, SDLK_c);
    input.addKeyboardAction(selectItemLeft, SDLK_a);
    input.addKeyboardAction(selectItemRight, SDLK_s);

    input.addJoyButtonAction(up, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
    input.addJoyButtonAction(down, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    input.addJoyButtonAction(left, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    input.addJoyButtonAction(right, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    input.addJoyButtonAction(jump, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A);
    input.addJoyButtonAction(modifyBlockFront, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X);
    input.addJoyButtonAction(modifyBlockBack, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B);
    input.addJoyButtonAction(selectItemLeft, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    input.addJoyButtonAction(selectItemRight, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
}