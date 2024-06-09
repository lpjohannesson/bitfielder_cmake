#include "option_list.h"
#include "client/client.h"

using namespace bf;

void OptionList::updateMesh() {
    Font &font = client->font;

    FontProperties fontProperties;

    fontProperties.spriteBatch = &client->fontSpriteBatch;
    fontProperties.position = { 0.0f, 0.0f };
    fontProperties.centered = true;

    for (int i = 0; i < options.size(); i++) {
        const std::string &option = options[i];

        fontProperties.position.y = i * font.lineHeight;

        if (i == selectedIndex) {
            fontProperties.color = { 1.0f, 1.0f, 0.0f, 1.0f };
        }
        else {
            fontProperties.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        }

        font.drawText(option, fontProperties);
    }

    client->fontSpriteBatch.uploadMesh(mesh);
}

void OptionList::setOptions(const std::vector<std::string> &options) {
    this->options = options;
    updateMesh();
}

void OptionList::update() {
    
}

void OptionList::render() {
    client->spriteRenderer.renderMesh(mesh, client->spriteProgram, client->fontTexture);
}