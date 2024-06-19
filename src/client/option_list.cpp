#include "option_list.h"
#include "client/client.h"
#include "core/direction.h"

using namespace bf;

void OptionList::updateMesh() {
    Font &font = client->font;

    FontProperties fontProperties;

    fontProperties.spriteBatch = &client->fontSpriteBatch;
    fontProperties.position = { 0.0f, 0.0f };
    fontProperties.centered = true;

    for (int i = 0; i < options.size(); i++) {
        const std::string &option = options[i];

        fontProperties.position.y = (float)i * font.lineHeight;

        if (i == selectedIndex) {
            fontProperties.color = { 1.0f, 0.5f, 0.0f, 1.0f };
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
    int selectDirection = 
        (int)client->clientInput.down.justPressed() -
        (int)client->clientInput.up.justPressed();
    
    if (selectDirection != 0) {
        selectedIndex = Direction::posmod(selectedIndex + selectDirection, (int)options.size());
        updateMesh();
    }

    if (client->clientInput.jump.justPressed()) {
        
    }
}

void OptionList::render() {
    client->spriteRenderer.renderMesh(mesh, client->spriteProgram, client->fontTexture);
}