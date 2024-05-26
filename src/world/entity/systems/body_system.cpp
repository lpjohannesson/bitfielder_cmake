#include "body_system.h"
#include "entity_system_impl.h"
#include "core/game_time.h"
#include "world/block/block_sample.h"

using namespace bf;

#define GET_COLLISION_AXIS(AXIS, OTHER_AXIS)\
    const Box2\
        &mover = movement.mover,\
        &collider = movement.collider;\
    \
    if (mover.start.OTHER_AXIS >= collider.start.OTHER_AXIS + collider.size.OTHER_AXIS ||\
        mover.start.OTHER_AXIS + mover.size.OTHER_AXIS <= collider.start.OTHER_AXIS) {\
        /* Other axis not aligned */\
        return false;\
    }\
    \
    float wall;\
    \
    if (endPosition - mover.start.AXIS < 0.0f) {\
        /* Move negative */\
        \
        if (mover.start.AXIS + mover.size.AXIS < collider.start.AXIS + collider.size.AXIS) {\
            /* Already beyond */\
            return false;\
        }\
        \
        wall = collider.start.AXIS + collider.size.AXIS;\
        \
        if (endPosition > wall) {\
            /* Not far enough */\
            return false;\
        }\
    }\
    else {\
        /* Move positive */\
        \
        if (mover.start.AXIS > collider.start.AXIS) {\
            /* Already beyond */\
            return false;\
        }\
        \
        wall = collider.start.AXIS - mover.size.AXIS;\
        \
        if (endPosition < wall) {\
            /* Not far enough */\
            return false;\
        }\
    }\
    \
    /* Success */\
    endPosition = wall;\
    \
    return true;

#define MOVE_AXIS(AXIS, OTHER_AXIS, BLOCK_SAMPLE_START, BLOCK_SAMPLE_END, BLOCK_X, BLOCK_Y, GET_COLLISION_FUNCTION)\
    if (body.velocity.AXIS == 0.0f) {\
        /* Not moving */\
        return;\
    }\
    \
    BodyMovement movement;\
    \
    Box2\
        &mover = movement.mover,\
        &collider = movement.collider;\
    \
    mover = { position, body.size };\
    \
    float endPosition = position.AXIS + body.velocity.AXIS * gameTime.getDeltaTime();\
    bool collided = false;\
    \
    /* Collide blocks */\
    \
    /* Find extents along axis */\
    int blockForwardStart, blockForwardEnd;\
    \
    if (body.velocity.AXIS < 0.0f) {\
        blockForwardStart = glm::floor(position.AXIS);\
        blockForwardEnd = glm::floor(endPosition);\
    }\
    else {\
        blockForwardStart = glm::floor(position.AXIS + body.size.AXIS);\
        blockForwardEnd = glm::floor(endPosition + body.size.AXIS);\
    }\
    \
    int blockForwardSign = glm::sign(blockForwardEnd - blockForwardStart);\
    \
    /* Find extents along other axis */\
    int blockSideStart = glm::floor(mover.start.OTHER_AXIS);\
    int blockSideEnd = glm::floor(mover.start.OTHER_AXIS + mover.size.OTHER_AXIS);\
    \
    /* Get block sample */\
    int blockSampleStart = glm::min(BLOCK_SAMPLE_START, BLOCK_SAMPLE_END);\
    int blockSampleEnd = glm::max(BLOCK_SAMPLE_START, BLOCK_SAMPLE_END);\
    \
    BlockSample blockSample(world.map, blockSampleStart, blockSampleEnd);\
    \
    /* Move forward until end */\
    int blockForward = blockForwardStart;\
    \
    while (true) {\
        for (int blockSide = blockSideStart; blockSide <= blockSideEnd; blockSide++) {\
            glm::ivec2 blockPosition = { BLOCK_X, BLOCK_Y };\
            \
            int blockIndex = blockSample.sampleBlockIndex(blockPosition);\
            entt::entity block = world.blocks.getBlock(blockIndex);\
            \
            /* TODO: add collision component to blocks */\
            if (blockIndex == 0) {\
                continue;\
            }\
            \
            collider = { blockPosition, { 1.0f, 1.0f } };\
            \
            collided |= GET_COLLISION_FUNCTION(movement, endPosition);\
            \
            if (collided) {\
                break;\
            }\
        }\
        \
        if (collided) {\
            break;\
        }\
        \
        /* Advance or stop if reached end */\
        if (blockForward == blockForwardEnd) {\
            break;\
        }\
        \
        blockForward += blockForwardSign;\
    }\
    \
    /* End */\
    position.AXIS = endPosition;\
    \
    if (collided) {\
        body.velocity.AXIS = 0.0f;\
    }

bool BodySystem::getCollisionX(const BodyMovement &movement, float &endPosition) {
    GET_COLLISION_AXIS(x, y)
}

bool BodySystem::getCollisionY(const BodyMovement &movement, float &endPosition) {
    GET_COLLISION_AXIS(y, x)
}

void BodySystem::moveX(World &world, glm::vec2 &position, BodyComponent &body) {
    MOVE_AXIS(x, y, blockForwardStart, blockForwardEnd, blockForward, blockSide, getCollisionX)
}

void BodySystem::moveY(World &world, glm::vec2 &position, BodyComponent &body) {
    MOVE_AXIS(y, x, blockSideStart, blockSideEnd, blockSide, blockForward, getCollisionY)
}

void BodySystem::update(World &world) {
    auto view = world.entities.registry.view<PositionComponent, BodyComponent>();

    for (auto [entity, position, body] : view.each()) {
        moveX(world, position.position, body);
        moveY(world, position.position, body);
    }
}