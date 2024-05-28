#pragma once

namespace bf {
	struct LocalPlayerComponent {
		bool jumpStopped = false;
		float floorTime = 0.0f;
	};
}