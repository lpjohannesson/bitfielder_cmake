#pragma once
#include "render_system.h"

namespace bf {
	class SpriteAnimatorSystem : public RenderSystem {
    public:
        void render(WorldScene &scene) override;
	};
}