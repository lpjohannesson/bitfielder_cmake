#include "option_list.h"
#include <glm/gtx/common.hpp>
#include "list_option.h"
#include "client/client.h"
#include "core/direction.h"
#include "core/game_time.h"

using namespace bf;

ListOption *OptionList::getPressedOption() const {
    if (client->clientInput.jump.justPressed()) {
        return options[selectedIndex];
    }

    return nullptr;
}

void OptionList::setOptions(const std::vector<ListOption*> &options) {
    this->options = options;
}

void OptionList::updateMesh() {
    // Reset position
    renderer->optionPosition = { 0.0f, 0.0f };

    // Render header text
    if (!headerText.empty()) {
        renderer->renderText(headerText, renderer->headerFontProperties);
    }

    for (int i = 0; i < options.size(); i++) {
        ListOption *option = options[i];

        // Select color
        if (i == selectedIndex) {
            renderer->optionFontProperties.color = { 1.0f, 0.5f, 0.0f, 1.0f };
        }
        else {
            renderer->optionFontProperties.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        }

        option->render(*renderer);
    }

    client->fontSpriteBatch.uploadMesh(mesh);
}

void OptionList::update() {
    int selectDirection = 
        (int)client->clientInput.down.justPressed() -
        (int)client->clientInput.up.justPressed();
    
    if (selectDirection != 0) {
        selectedIndex = Direction::posmod(selectedIndex + selectDirection, (int)options.size());
        updateMesh();
    }

    ListOption
        *hoveredOption = options[selectedIndex],
        *pressedOption = getPressedOption();
   
   hoveredOption->optionHoverUpdate(*this);

    if (pressedOption != nullptr) {
        pressedOption->optionPressed(*this);
    }

    // Advance cursor progress
    renderer->cursorProgress = glm::fmod(renderer->cursorProgress + gameTime.getDeltaTime(), 1.0f);
}

void OptionList::render() {
    client->spriteRenderer.renderMesh(mesh, client->spriteProgram, client->fontTexture);
}

OptionList::OptionList(OptionListRenderer &renderer) : mesh(client->spriteRenderer) {
    this->renderer = &renderer;
}