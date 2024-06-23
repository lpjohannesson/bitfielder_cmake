#pragma once
#include <vector>
#include <string>
#include "gfx/sprite/sprite_mesh.h"
#include "client/client.h"
#include "option_list_renderer.h"

namespace bf {
    class ListOption;

    class OptionList {
    private:
        OptionListRenderer *renderer = nullptr;

    public:
        std::vector<ListOption*> options;
        int selectedIndex = 0;

        inline OptionListRenderer *getRenderer() const { return renderer; }

        SpriteMesh mesh;

        ListOption *getPressedOption() const;

        void setOptions(const std::vector<ListOption*> &options);

        void updateMesh();

        void update();
        void render();

        OptionList(OptionListRenderer &renderer);
    };
}