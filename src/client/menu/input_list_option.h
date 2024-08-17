#pragma once
#include <string>
#include "list_option.h"

namespace bf {
    class InputListOption : public ListOption {
    public:
        bool inputActive = false;
        std::string headerText, inputText;

        void optionPressed(OptionList &optionList) override;
        void optionHoverUpdate(OptionList &optionList) override;

        void render(OptionListRenderer &renderer) override;
    };
}