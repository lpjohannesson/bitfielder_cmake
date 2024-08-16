#include "option_list_renderer.h"
#include "option_list.h"
#include "list_option.h"
#include "client/client.h"

using namespace bf;

void OptionListRenderer::renderText(std::string text, FontProperties &properties) {
    properties.position = optionPosition;
    client->font.drawText(text, properties, client->fontSpriteBatch);

    optionPosition.y += client->font.lineHeight;
}

OptionListRenderer::OptionListRenderer() {
    optionFontProperties.centered = true;
    headerFontProperties.centered = true;
    cursorFontProperties.centered = false;
}