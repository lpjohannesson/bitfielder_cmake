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
}