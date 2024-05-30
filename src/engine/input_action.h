#pragma once

namespace bf {
    class Input;

    class InputAction {
    public:
        bool pressed = false, lastPressed = false;

        bool justPressed() const;
        bool justReleased() const;

        InputAction(Input &input);
        InputAction();
    };
}