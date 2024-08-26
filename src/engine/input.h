#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "input_action.h"

namespace bf {
        template <typename T>
        class InputActionMap {
        private:
                std::unordered_map<T, InputAction*> actions;

        public:
                inline void addAction(InputAction &action, T key) {
                        actions.emplace(key, &action);
                }

                inline InputAction *getAction(T key) const {
                        auto foundAction = actions.find(key);

                        if (foundAction == actions.end()) {
                                return nullptr;
                        }
                        
                        return foundAction->second;
                }

                inline void keyDown(T key) {
                        InputAction *action = getAction(key);

                        if (action == nullptr) {
                                return;
                        }

                        action->value = 1.0f;
                }

                inline void keyUp(T key) {
                        InputAction *action = getAction(key);

                        if (action == nullptr) {
                                return;
                        }

                        action->value = 0.0f;
                }
        };

        class Input {
        private:
                bool textMode = false;
                int textCursor = 0;

                InputAction *getControllerAction(Uint8 button);

                void applyJoyAxis(float value, InputAction *action);
                void applyStickInput(float value, SDL_GameControllerButton negativeButton, SDL_GameControllerButton positiveButton);

                void keyDown(SDL_Keycode key);
                void keyUp(SDL_Keycode key);

                void joyButtonDown(SDL_GameControllerButton button);
                void joyButtonUp(SDL_GameControllerButton button);

                void joyAxisMotion(SDL_GameControllerAxis axis, Sint32 value);

        public:
                std::vector<InputAction*> actions;
                std::string inputText;

                InputActionMap<SDL_Keycode> keyboard;
                InputActionMap<SDL_GameControllerButton> joyButton;
                InputActionMap<SDL_GameControllerAxis> joyAxis;

                bool joystickMode = false;

                float axisDeadzone = 0.5f;
                
                inline int getTextCursor() const { return textCursor; }

                inline bool getTextMode() const { return textMode; }

                void startTextMode(std::string inputText);
                void endTextMode();

                void processEvent(SDL_Event &event);

                void update();
        };
}