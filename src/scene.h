#pragma once

namespace bf {
	class Scene {
	public:
		inline virtual void update() {}
		inline virtual void render() {}

		inline virtual void start() {}
		inline virtual void end() {}
	};
}