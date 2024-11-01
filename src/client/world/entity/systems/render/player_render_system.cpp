#include "player_render_system.h"
#include "client/scenes/world_scene.h"
#include "client/world/entity/components/animator_component.h"
#include "client/world/entity/components/skin_component.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/animation_component.h"
#include "world/entity/components/aim_component.h"
#include "sprite_system.h"
#include "gfx/core/shader.h"

using namespace bf;

void PlayerRenderSystem::render(WorldScene &scene) {
    skinProgram.setTransform(scene.worldRenderer.viewTransform);

    entt::registry &entityRegistry = scene.world.entities.registry;
    auto view = entityRegistry.view<PositionComponent, AnimationComponent, AnimatorComponent, AimComponent, SkinComponent>();

    for (auto [entity, position, animation, animator, aim, skin] : view.each()) {
        // Get sprite sets
        Animation &currentAnimation = *animator.animations->getAnimation(animation.index);
        int frame = currentAnimation.getFrame(animator.time);

        SpriteSet &skinSprites = scene.clientContent.playerSkinSprites.getFromAim(aim.aim);
        SpriteSet &overlaySprites = scene.clientContent.playerOverlaySprites.getFromAim(aim.aim);

        // Draw skin with unique texture
        sprite.uvBox = skinSprites.boxes.at(frame);
        SpriteSystem::drawSprite(entity, sprite, position, scene, skinSpriteBatch.createSprite());

        skinSpriteBatch.uploadMesh(mesh);

        glActiveTexture(GL_TEXTURE3);
	    glBindTexture(GL_TEXTURE_2D, skin.texture->getGLTexture());

        client->spriteRenderer.renderMesh(mesh, skinProgram);

        // Draw overlay to batch
        sprite.uvBox = overlaySprites.boxes.at(frame);
        SpriteSystem::drawSprite(entity, sprite, position, scene, overlaySpriteBatch.createSprite());
    }

    overlaySpriteBatch.uploadMesh(mesh);
    client->spriteRenderer.renderMesh(mesh, client->spriteProgram);
}

PlayerRenderSystem::PlayerRenderSystem() : mesh(client->spriteRenderer) {
    // Create sprite
    sprite.size = { 1.0f, 1.0f };
    sprite.offset = glm::vec2(-4.0f, -2.0f) / 16.0f;

    // Create skin shader
    GLuint
        glVertexShader = glCreateShader(GL_VERTEX_SHADER),
        glSkinShader = glCreateShader(GL_FRAGMENT_SHADER);

	Shader::compileShader(glVertexShader, "assets/shaders/vertex.glsl");
    Shader::compileShader(glSkinShader, "assets/shaders/skin.glsl");

    skinProgram.attachShader(glVertexShader);
    skinProgram.attachShader(glSkinShader);
    skinProgram.link();

	glDeleteShader(glVertexShader);
    glDeleteShader(glSkinShader);

    // Skip texture slot 2 for shadow buffer
    skinProgram.assignTexture(1, "fTexture");
    skinProgram.assignTexture(3, "fSkinTexture");

    skinSpriteBatch.defaultDepth = overlaySpriteBatch.defaultDepth = WorldRenderer::ENTITY_DEPTH;
}