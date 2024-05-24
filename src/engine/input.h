#pragma once
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>

namespace bf {
    class Input;

    class InputAction {
    public:
        bool pressed = false, lastPressed = false;

        bool justPressed() const;
        bool justReleased() const;

        InputAction();
    };

	class Input {
    private:
        std::unordered_map<SDL_Keycode, InputAction*> keyboardActions;

	public:
        std::vector<InputAction*> actions;

        InputAction *getKeyboardAction(SDL_Keycode key) const;
        void addKeyboardAction(InputAction &action, SDL_Keycode key);

        void keyDown(SDL_Keycode key);
        void keyUp(SDL_Keycode key);

        void update();
	};
}