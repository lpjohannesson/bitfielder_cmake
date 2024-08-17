#pragma once

namespace bf {
    class Input;

    class InputAction {
    public:
        float value = 0.0f, lastValue = 0.0f;

        bool pressed() const;
        bool lastPressed() const;

        bool justPressed() const;
        bool justReleased() const;

        InputAction(Input &input);
        InputAction();
    };
}