#include "sprite_animator_system.h"
#include "client/scenes/world_scene.h"
#include "client/world/entity/components/sprite_component.h"
#include "client/world/entity/components/sprite_set_component.h"
#include "client/world/entity/components/animator_component.h"
#include "world/entity/components/animation_component.h"

using namespace bf;

void SpriteAnimatorSystem::render(WorldScene &scene) {
    auto view = scene.world.entities.registry.view<SpriteComponent, SpriteSetComponent, AnimationComponent, AnimatorComponent>();

    for (auto [entity, sprite, sprites, animation, animator] : view.each()) {
        Animation &currentAnimation = *animator.animations->getAnimation(animation.index);
        int frame = currentAnimation.getFrame(animator.time);
        sprite.uvBox = sprites.sprites->boxes.at(frame);
    }
}