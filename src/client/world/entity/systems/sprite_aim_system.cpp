#include "sprite_aim_system.h"

using namespace bf;

SpriteFrames *SpriteAimSystem::getAimFrames(const SpriteAimComponent &spriteAim, int aim) {
    switch (aim) {
    case 0:
        return spriteAim.forwardFrames;
    
    case -1:
        return spriteAim.upFrames;

    case 1:
        return spriteAim.downFrames;

    default:
        return spriteAim.forwardFrames;
    }
}