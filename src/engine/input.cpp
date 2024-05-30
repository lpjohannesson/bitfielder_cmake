#include "input.h"
#include "engine/engine.h"

using namespace bf;

void Input::applyStickInput(float &axis, const float &nextAxis, SDL_GameControllerButton negativeButton, SDL_GameControllerButton positiveButton) {
    if (nextAxis <= -joyDeadzone && axis > -joyDeadzone) {
        joyButtonDown(negativeButton);
    }
    else if (axis <= -joyDeadzone && nextAxis > -joyDeadzone) {
        joyButtonUp(negativeButton);
    }

    if (nextAxis >= joyDeadzone && axis < joyDeadzone) {
        joyButtonDown(positiveButton);
    }
    else if (axis >= joyDeadzone && nextAxis < joyDeadzone) {
        joyButtonUp(positiveButton);
    }

    axis = nextAxis;
}

InputAction *Input::getKeyboardAction(SDL_Keycode key) const {
    auto foundAction = keyboardActions.find(key);

    if (foundAction == keyboardActions.end()) {
        return nullptr;
    }
    
    return foundAction->second;
}

void Input::addKeyboardAction(InputAction &action, SDL_Keycode key) {
    keyboardActions.emplace(key, &action);
}

InputAction *Input::getJoyButtonAction(SDL_GameControllerButton button) const {
    auto foundAction = joyButtonActions.find(button);

    if (foundAction == joyButtonActions.end()) {
        return nullptr;
    }
    
    return foundAction->second;
}

void Input::addJoyButtonAction(InputAction &action, SDL_GameControllerButton button) {
    joyButtonActions.emplace(button, &action);
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

void Input::joyButtonDown(SDL_GameControllerButton button) {
    InputAction *action = getJoyButtonAction(button);

    if (action == nullptr) {
        return;
    }

    action->pressed = true;
}

void Input::joyButtonUp(SDL_GameControllerButton button) {
    InputAction *action = getJoyButtonAction(button);

    if (action == nullptr) {
        return;
    }

    action->pressed = false;
}

void Input::joyAxisMotion(SDL_GameControllerAxis axisType, Sint32 value) {
    // Normalize value
    float nextAxis = value / 32767.0f;

    switch (axisType) {
    case SDL_CONTROLLER_AXIS_LEFTX:
        applyStickInput(joyStick.x, nextAxis, SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        break;

    case SDL_CONTROLLER_AXIS_LEFTY:
        applyStickInput(joyStick.y, nextAxis, SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        break;
    }
}

void Input::update() {
    // Update last pressed actions
    for (InputAction *action : actions) {
        action->lastPressed = action->pressed;
    }
}