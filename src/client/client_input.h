#pragma once
#include "engine/input.h"

namespace bf {
	class ClientInput {
	public:
        InputAction up, down, left, right;

        inline ClientInput(Input &input) :
            up(input),
            down(input),
            left(input),
            right(input) {}
	};
}