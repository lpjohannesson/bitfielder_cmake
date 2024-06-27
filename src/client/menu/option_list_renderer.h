#pragma once
#include <glm/glm.hpp>
#include "gfx/text/font.h"

namespace bf {
    class OptionList;

    class OptionListRenderer {
    public:
        FontProperties optionFontProperties, headerFontProperties, cursorFontProperties;
        glm::vec2 optionPosition;
        float cursorProgress = 0.0f;

        void renderText(std::string text, FontProperties &properties);

        OptionListRenderer();
    };
}