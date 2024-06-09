#pragma once
#include <vector>
#include <string>
#include "gfx/sprite/sprite_mesh.h"
#include "client/client.h"

namespace bf {
    class OptionList {
    private:
        std::vector<std::string> options;
        int selectedIndex = 0;
        
        SpriteMesh mesh;

        void updateMesh();

    public:
        void setOptions(const std::vector<std::string> &options);

        void update();

        void render();

        inline OptionList() : mesh(client->spriteRenderer) {}
    };
}