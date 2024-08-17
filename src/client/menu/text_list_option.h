#pragma once
#include <string>
#include "list_option.h"

namespace bf {
    class TextListOption : public ListOption {
    public:
        std::string text;

        void render(OptionListRenderer &renderer) override;
    };
}