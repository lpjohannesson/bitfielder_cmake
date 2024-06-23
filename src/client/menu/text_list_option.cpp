#include "text_list_option.h"

using namespace bf;

void TextListOption::render(OptionListRenderer &renderer) {
    renderer.renderText(text);
}