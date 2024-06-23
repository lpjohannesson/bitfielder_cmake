#include "option_list_renderer.h"
#include "option_list.h"
#include "list_option.h"
#include "client/client.h"

using namespace bf;

void OptionListRenderer::renderText(std::string text) {
    fontProperties.position = optionPosition;
    client->font.drawText(text, fontProperties);

    optionPosition.y += client->font.lineHeight;
}

OptionListRenderer::OptionListRenderer() {
    fontProperties.spriteBatch = &client->fontSpriteBatch;
    cursorFontProperties.spriteBatch = &client->fontSpriteBatch;

    fontProperties.centered = true;
    cursorFontProperties.centered = false;
}