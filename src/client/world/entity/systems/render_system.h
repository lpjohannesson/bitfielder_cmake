#pragma once

namespace bf {
    class WorldScene;
    
	class RenderSystem {
    public:
        inline virtual void render(const WorldScene &scene) {}
    };
}