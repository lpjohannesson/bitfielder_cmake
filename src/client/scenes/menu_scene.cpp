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
    client->font.drawText("the quick brown fox jumps over the lazy dog", textSpriteBatch, { 0.0f, 0.0f });
    client->font.drawText("THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", textSpriteBatch, { 0.0f, 1.0f });
    client->font.drawText("1234567890", textSpriteBatch, { 0.0f, 2.0f });
    client->font.drawText("welcome to bitfielder! this is some test text...", textSpriteBatch, { 0.0f, 3.0f });

    textSpriteBatch.uploadMesh(textMesh);
}