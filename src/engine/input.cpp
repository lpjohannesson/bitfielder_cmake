#include "input.h"
#include "engine/engine.h"

using namespace bf;

InputAction *Input::getControllerAction(Uint8 button) {
#ifdef NX
    switch (button) {
    case 0:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A);
    case 1:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B);
    case 2:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X);
    case 3:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y);
    case 4:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK);
    case 5:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK);
    case 6:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    case 7:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    case 8:
        return joyAxis.getAction(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT);
    case 9:
        return joyAxis.getAction(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    case 10:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START);
    case 11:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK);
    case 12:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    case 13:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
    case 14:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    case 15:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    case 16:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    case 17:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
    case 18:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    case 19:
        return joyButton.getAction(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    }
#endif

    return nullptr;
}

void Input::applyJoyAxis(float value, InputAction *action) {
    if (action == nullptr) {
        return;
    }

    if (glm::abs(value) < axisDeadzone) {
        if (joystickMode) {
            action->value = 0.0f;
        }
    }
    else {
        action->value = value;
        joystickMode = true;
    }
}

void Input::applyStickInput(float value, SDL_GameControllerButton negativeButton, SDL_GameControllerButton positiveButton) {
    applyJoyAxis(glm::min(0.0f, value), joyButton.getAction(negativeButton));
    applyJoyAxis(glm::max(0.0f, value), joyButton.getAction(positiveButton));
}

void Input::keyDown(SDL_Keycode key) {
    if (textMode) {
        return;
    }

    keyboard.keyDown(key);
    joystickMode = false;
}

void Input::keyUp(SDL_Keycode key) {
    if (textMode) {
        return;
    }

    if (joystickMode) {
        return;
    }

    keyboard.keyUp(key);
}

void Input::joyButtonDown(SDL_GameControllerButton button) {
    if (textMode) {
        return;
    }

    joyButton.keyDown(button);
    joystickMode = true;
}

void Input::joyButtonUp(SDL_GameControllerButton button) {
    if (textMode) {
        return;
    }

    if (!joystickMode) {
        return;
    }

    joyButton.keyUp(button);
}

void Input::joyAxisMotion(SDL_GameControllerAxis axis, Sint32 value) {
    if (textMode) {
        return;
    }

    // Normalize value
    float axisValue = value / 32767.0f;

    // Apply stick to dpad
    switch (axis) {
        case SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX:
            applyStickInput(axisValue,
                SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT,
                SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

            break;

        case SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY:
            applyStickInput(axisValue,
                SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP,
                SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);

            break;
        
        default:
            applyJoyAxis(axisValue, joyAxis.getAction(axis));

            break;
    }
}

void Input::startTextMode(std::string inputText) {
    textMode = true;
    SDL_StartTextInput();

    this->inputText = inputText;
    textCursor = (int)inputText.length();

    // Release all actions
    for (InputAction *action : actions) {
        action->value = 0.0f;
    }
}

void Input::endTextMode() {
    textMode = false;
    SDL_StopTextInput();
}

void Input::processEvent(SDL_Event &event) {
    if (textMode) {
        switch (event.type) {
        case SDL_TEXTINPUT: {
            inputText.insert(textCursor, event.text.text);

            int length = (int)strlen(event.text.text);
            textCursor += length;
            
            break;
        }
        
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
            case SDLK_RETURN:
            case SDLK_ESCAPE: {
                endTextMode();
                break;
            }
                
            case SDLK_BACKSPACE: {
                if (inputText.empty()) {
                    break;
                }

                if (textCursor == 0) {
                    break;
                }

                textCursor--;
                inputText.erase(textCursor, 1);

                break;
            }

            case SDLK_LEFT: {
                textCursor = glm::max(textCursor - 1, 0);
                break;
            }

            case SDLK_RIGHT: {
                textCursor = glm::min(textCursor + 1, (int)inputText.length());
                break;
            }
            }
        }
        }
    }
    else {
        switch (event.type) {
        case SDL_KEYDOWN:
            keyDown(event.key.keysym.sym);
            break;
        
        case SDL_KEYUP:
            keyUp(event.key.keysym.sym);
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            joyButtonDown((SDL_GameControllerButton)event.cbutton.button);
            break;

        case SDL_CONTROLLERBUTTONUP:
            joyButtonUp((SDL_GameControllerButton)event.cbutton.button);
            break;

        case SDL_CONTROLLERAXISMOTION:
            joyAxisMotion((SDL_GameControllerAxis)event.caxis.axis, event.caxis.value);
            break;

        case SDL_JOYBUTTONDOWN: {
            InputAction *action = getControllerAction(event.jbutton.button);

            if (action == nullptr) {
                break;
            }

            action->value = 1.0f;
            break;
        }
            
        case SDL_JOYBUTTONUP: {
            InputAction *action = getControllerAction(event.jbutton.button);

            if (action == nullptr) {
                break;
            }

            action->value = 0.0f;
            break;
        }
        }
    }
}

void Input::update() {
    if (textMode && !SDL_IsTextInputActive()) {
        endTextMode();
    }

    // Update last pressed actions
    for (InputAction *action : actions) {
        action->lastValue = action->value;
    }
}