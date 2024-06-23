#include "input.h"
#include "engine/engine.h"

using namespace bf;

SDL_GameControllerButton Input::getControllerJoyButton(Uint8 button) {
	SDL_GameControllerButton buttons[] {
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
	};

	return buttons[button];
}

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

void Input::keyDown(SDL_Keycode key) {
    if (textMode) {
        return;
    }

    InputAction *action = getKeyboardAction(key);

    if (action == nullptr) {
        return;
    }

    action->pressed = true;
}

void Input::keyUp(SDL_Keycode key) {
    if (textMode) {
        return;
    }

    InputAction *action = getKeyboardAction(key);

    if (action == nullptr) {
        return;
    }

    action->pressed = false;
}

void Input::joyButtonDown(SDL_GameControllerButton button) {
    if (textMode) {
        return;
    }

    InputAction *action = getJoyButtonAction(button);

    if (action == nullptr) {
        return;
    }

    action->pressed = true;
}

void Input::joyButtonUp(SDL_GameControllerButton button) {
    if (textMode) {
        return;
    }

    InputAction *action = getJoyButtonAction(button);

    if (action == nullptr) {
        return;
    }

    action->pressed = false;
}

void Input::joyAxisMotion(SDL_GameControllerAxis axisType, Sint32 value) {
    if (textMode) {
        return;
    }

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

void Input::startTextMode(std::string inputText) {
    textMode = true;
    SDL_StartTextInput();

    this->inputText = inputText;
    textCursor = inputText.length();

    // Release all actions
    for (InputAction *action : actions) {
        action->pressed = false;
    }
}

void Input::endTextMode() {
    textMode = false;
    SDL_StopTextInput();
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

                inputText.erase(textCursor - 1, 1);
                textCursor = glm::min(textCursor - 1, (int)inputText.length());

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

        case SDL_JOYBUTTONDOWN:
            joyButtonDown(getControllerJoyButton(event.jbutton.button));
            break;

        case SDL_JOYBUTTONUP:
            joyButtonUp(getControllerJoyButton(event.jbutton.button));
            break;
        }
    }
    
}

void Input::update() {
    if (textMode && !SDL_IsTextInputActive()) {
        endTextMode();
    }

    // Update last pressed actions
    for (InputAction *action : actions) {
        action->lastPressed = action->pressed;
    }
}