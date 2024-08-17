#include "input_action.h"
#include "engine/engine.h"

using namespace bf;

bool InputAction::pressed() const {
    return value != 0.0f;
}

bool InputAction::lastPressed() const {
    return lastValue != 0.0f;
}

bool InputAction::justPressed() const {
    return pressed() && !lastPressed();
}

bool InputAction::justReleased() const {
    return !pressed() && lastPressed();
}

InputAction::InputAction(Input &input) {
    input.actions.push_back(this);
}

InputAction::InputAction() : InputAction(engine->input) {}