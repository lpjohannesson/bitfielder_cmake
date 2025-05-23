#pragma once
#include "world.h"

namespace bf {
	class WorldFile {
    public:
        static void saveWorld(World &world, const std::string folderPath);
        static bool loadWorld(World &world, const std::string folderPath);
    };
}