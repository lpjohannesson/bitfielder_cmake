#pragma once
#include <glm/glm.hpp>
#include "gfx/text/font.h"
#include "option_list.h"

namespace bf {
    class ListOption {
    public:
        inline virtual void optionPressed(OptionList &optionList) {}
        inline virtual void optionHoverUpdate(OptionList &optionList) {}

        inline virtual void render(OptionListRenderer &renderer) {}
    };
}