#pragma once

namespace bf {
    class WorldScene;
    
	class ClientEntitySystem {
    public:
        inline virtual void update(WorldScene &scene) {}
    };
}