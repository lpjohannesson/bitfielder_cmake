#include "menu_scene.h"
#include "engine/engine.h"

using namespace bf;

void MenuScene::render() {
    engine->renderer.clearScreen({ 0.5f, 0.25f, 0.5f, 1.0f });

    client->spriteProgram.setTransform(glm::scale(engine->getWindowTransform(), glm::vec3(48.0f)));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, client->fontTexture.getGLTexture());

    client->spriteRenderer.renderMesh(textMesh, client->spriteProgram);
}

void MenuScene::start() {
    client->font.drawText("welcome to bitfielder!", textSpriteBatch, { 0.0f, 0.0f });
    client->font.drawText("This is a test.", textSpriteBatch, { 0.0f, 1.0f });

    textSpriteBatch.uploadMesh(textMesh);
}