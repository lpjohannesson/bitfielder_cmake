#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "input_action.h"

namespace bf {
        class Input {
        private:
                float joyDeadzone = 0.5f;

                std::unordered_map<SDL_Keycode, InputAction*> keyboardActions;
                std::unordered_map<SDL_GameControllerButton, InputAction*> joyButtonActions;

                glm::vec2 joyStick{};
                
                bool textMode = false;
                int textCursor = 0;

                SDL_GameControllerButton getControllerJoyButton(Uint8 button);
                
                void applyStickInput(float &axis, const float &nextAxis, SDL_GameControllerButton negativeButton, SDL_GameControllerButton positiveButton);

                void keyDown(SDL_Keycode key);
                void keyUp(SDL_Keycode key);

                void joyButtonDown(SDL_GameControllerButton button);
                void joyButtonUp(SDL_GameControllerButton button);

                void joyAxisMotion(SDL_GameControllerAxis axisType, Sint32 value);

        public:
                std::vector<InputAction*> actions;
                std::string inputText;
                
                inline int getTextCursor() const { return textCursor; }

                inline bool getTextMode() const { return textMode; }

                void startTextMode(std::string inputText);
                void endTextMode();

                InputAction *getKeyboardAction(SDL_Keycode key) const;
                void addKeyboardAction(InputAction &action, SDL_Keycode key);

                InputAction *getJoyButtonAction(SDL_GameControllerButton button) const;
                void addJoyButtonAction(InputAction &action, SDL_GameControllerButton button);

                void processEvent(SDL_Event &event);

                void update();
        };
}