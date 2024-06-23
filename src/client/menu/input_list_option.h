#pragma once
#include <string>
#include "list_option.h"

namespace bf {
    class InputListOption : public ListOption {
    public:
        bool inputActive = false;
        std::string headerText, inputText;

        virtual void optionPressed(OptionList &optionList) override;
        virtual void optionHoverUpdate(OptionList &optionList) override;

        virtual void render(OptionListRenderer &renderer) override;
    };
}