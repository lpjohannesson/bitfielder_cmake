#include "input.h"

using namespace bf;

bool InputAction::justPressed() const {
    return pressed && !lastPressed;
}

bool InputAction::justReleased() const {
    return !pressed && lastPressed;
}

InputAction::InputAction(Input &input) {
    input.actions.push_back(this);
}

InputAction *Input::getKeyboardAction(SDL_Keycode key) const {
    auto foundKeyboardAction = keyboardActions.find(key);

    if (foundKeyboardAction == keyboardActions.end()) {
        return nullptr;
    }
    
    return foundKeyboardAction->second;
}

void Input::addKeyboardAction(InputAction *action, SDL_Keycode key) {
    keyboardActions.emplace(key, action);
}

void Input::keyDown(SDL_Keycode key) {
    InputAction *action = getKeyboardAction(key);

    if (action == nullptr) {
        return;
    }

    action->pressed = true;
}

void Input::keyUp(SDL_Keycode key) {
    InputAction *action = getKeyboardAction(key);

    if (action == nullptr) {
        return;
    }

    action->pressed = false;
}

void Input::update() {
    // Update last pressed actions
    for (InputAction *action : actions) {
        action->lastPressed = action->pressed;
    }
}