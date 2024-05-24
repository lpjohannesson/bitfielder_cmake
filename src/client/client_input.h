#pragma once
#include "engine/input.h"

namespace bf {
	class ClientInput {
	public:
        InputAction up, down, left, right;

        ClientInput();
	};
}