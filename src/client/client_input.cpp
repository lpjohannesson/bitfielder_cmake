#include "client_input.h"
#include "engine/engine.h"

using namespace bf;

ClientInput::ClientInput() {
    Input &input = engine->input;

    input.addKeyboardAction(up, SDLK_w);
    input.addKeyboardAction(down, SDLK_s);
    input.addKeyboardAction(left, SDLK_a);
    input.addKeyboardAction(right, SDLK_d);
}