#pragma once
#include <glm/glm.hpp>

namespace bf {
	class BlockMapElement {
    private:
		int mapIndex;

    public:
        inline int getMapIndex() const { return mapIndex; }

		BlockMapElement(int mapIndex);
	};
}