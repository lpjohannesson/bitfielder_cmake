#include "input_list_option.h"
#include "client/client.h"

using namespace bf;

void InputListOption::optionPressed(OptionList &optionList) {
    inputActive = true;
    engine->input.startTextMode(inputText);

    OptionListRenderer *renderer = optionList.getRenderer();

    renderer->cursorProgress = 0.0f;
}

void InputListOption::optionHoverUpdate(OptionList &optionList) {
    if (!inputActive) {
        return;
    }

    // End input
    if (!engine->input.getTextMode()) {
        inputActive = false;
        optionList.updateMesh();
        return;
    }

    inputText = engine->input.inputText;
    optionList.updateMesh();
}

void InputListOption::render(OptionListRenderer &renderer) {
    renderer.renderText(headerText);
    renderer.renderText(inputText);

    // Render cursor
    if (!inputActive) {
        return;
    }

    if (renderer.cursorProgress > 0.5f) {
        return;
    }

    glm::vec2 renderPosition = client->font.getRenderPosition(inputText, renderer.fontProperties);
    renderer.cursorFontProperties.position = renderPosition;

    int textCursor = engine->input.getTextCursor();
    float cursorLength = client->font.getTextLength(inputText, textCursor);

    renderer.cursorFontProperties.position.x += cursorLength;

    client->font.drawText("_", renderer.cursorFontProperties);
}