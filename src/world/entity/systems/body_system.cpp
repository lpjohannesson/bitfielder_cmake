#include "body_system.h"
#include "entity_system_impl.h"
#include "engine/engine.h"
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

#define MOVE_AXIS(AXIS, OTHER_AXIS, BLOCK_SAMPLE_START, BLOCK_SAMPLE_END, BLOCK_POSITION, GET_COLLISION)\
    if (body.velocity.AXIS == 0.0f) {\
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
    float endPosition = position.AXIS + body.velocity.AXIS * engine->getDeltaTime();\
    bool collided = false;\
    \
    /* Collide blocks */\
    int blockForwardStart, blockForwardEnd;\
    \
    if (body.velocity.AXIS < 0.0f) {\
        blockForwardStart = glm::ceil(position.AXIS);\
        blockForwardEnd = glm::floor(endPosition);\
    }\
    else {\
        blockForwardStart = glm::floor(position.AXIS + body.size.AXIS);\
        blockForwardEnd = glm::ceil(endPosition + body.size.AXIS);\
    }\
    \
    int blockForwardSign = glm::sign(blockForwardEnd - blockForwardStart);\
    \
    int blockSideStart = glm::floor(mover.start.OTHER_AXIS);\
    int blockSideEnd = glm::ceil(mover.start.OTHER_AXIS + mover.size.OTHER_AXIS);\
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
            glm::ivec2 blockPosition = BLOCK_POSITION;\
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
            collided |= GET_COLLISION(movement, endPosition);\
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
        body.velocity.AXIS = 0;\
    }

bool BodySystem::getCollisionX(const BodyMovement &movement, float &endPosition) {
    GET_COLLISION_AXIS(x, y)
}

bool BodySystem::getCollisionY(const BodyMovement &movement, float &endPosition) {
    GET_COLLISION_AXIS(y, x)
}

void BodySystem::moveX(World &world, glm::vec2 &position, BodyComponent &body) {
    MOVE_AXIS(x, y, blockForwardStart, blockForwardEnd, glm::vec2(blockForward, blockSide), getCollisionX)
}

void BodySystem::moveY(World &world, glm::vec2 &position, BodyComponent &body) {
    MOVE_AXIS(y, x, blockSideStart, blockSideEnd, glm::vec2(blockSide, blockForward), getCollisionY)
}

void BodySystem::update(World &world) {
    auto view = world.entities.registry.view<PositionComponent, BodyComponent>();

    for (auto [entity, position, body] : view.each()) {
        moveX(world, position.position, body);
        moveY(world, position.position, body);
    }
}