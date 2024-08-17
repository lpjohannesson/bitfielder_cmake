#include "client_input.h"
#include "engine/engine.h"

using namespace bf;

ClientInput::ClientInput() {
    Input &input = engine->input;

    input.keyboard.addAction(up, SDLK_UP);
    input.keyboard.addAction(down, SDLK_DOWN);
    input.keyboard.addAction(left, SDLK_LEFT);
    input.keyboard.addAction(right, SDLK_RIGHT);
    input.keyboard.addAction(jump, SDLK_z);
    input.keyboard.addAction(modifyBlockFront, SDLK_x);
    input.keyboard.addAction(modifyBlockBack, SDLK_c);
    input.keyboard.addAction(selectItemLeft, SDLK_a);
    input.keyboard.addAction(selectItemRight, SDLK_s);
    input.keyboard.addAction(pause, SDLK_ESCAPE);
    input.keyboard.addAction(zoomIn, SDLK_EQUALS);
    input.keyboard.addAction(zoomOut, SDLK_MINUS);

    input.joyButton.addAction(up, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
    input.joyButton.addAction(down, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    input.joyButton.addAction(left, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    input.joyButton.addAction(right, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    input.joyButton.addAction(jump, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A);
    input.joyButton.addAction(selectItemLeft, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    input.joyButton.addAction(selectItemRight, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    input.joyButton.addAction(pause, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START);
    input.joyButton.addAction(zoomIn, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK);
    input.joyButton.addAction(zoomOut, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK);
    
    input.joyAxis.addAction(modifyBlockFront, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    input.joyAxis.addAction(modifyBlockBack, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT);
    input.joyAxis.addAction(cameraX, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX);
    input.joyAxis.addAction(cameraY, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY);
} 