#include "body_system.h"
#include "entity_system_impl.h"
#include "core/game_time.h"
#include "world/block/block_sample.h"
#include "../components/position_component.h"
#include "world/block/components/block_collision_component.h"
#include "world/block/components/one_way_block_component.h"

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

#define MOVE_AXIS(AXIS, OTHER_AXIS, BLOCK_SAMPLE_START, BLOCK_SAMPLE_END, BLOCK_X, BLOCK_Y, GET_COLLISION_FUNCTION, AXIS_BLOCK_CHECK)\
    if (velocity.velocity.AXIS == 0.0f) {\
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
    float endPosition = position.AXIS + (velocity.velocity.AXIS + velocity.oldVelocity.AXIS) * 0.5f * gameTime.getDeltaTime();\
    bool collided = false;\
    \
    /* Collide blocks */\
    \
    /* Find extents along axis */\
    int blockForwardStart, blockForwardEnd;\
    \
    if (velocity.velocity.AXIS < 0.0f) {\
        blockForwardStart = (int)glm::floor(position.AXIS);\
        blockForwardEnd = (int)glm::floor(endPosition);\
    }\
    else {\
        blockForwardStart = (int)glm::floor(position.AXIS + body.size.AXIS);\
        blockForwardEnd = (int)glm::floor(endPosition + body.size.AXIS);\
    }\
    \
    int blockForwardSign = glm::sign(blockForwardEnd - blockForwardStart);\
    \
    /* Find extents along other axis */\
    int blockSideStart = (int)glm::floor(mover.start.OTHER_AXIS);\
    int blockSideEnd = (int)glm::floor(mover.start.OTHER_AXIS + mover.size.OTHER_AXIS);\
    \
    /* Get block sample */\
    int blockSampleStart = glm::min(BLOCK_SAMPLE_START, BLOCK_SAMPLE_END);\
    int blockSampleEnd = glm::max(BLOCK_SAMPLE_START, BLOCK_SAMPLE_END);\
    \
    BlockSample<BlockChunk> blockSample;\
    blockSample.sampleBlocks(world.map, blockSampleStart, blockSampleEnd);\
    \
    /* Move forward until end */\
    int blockForward = blockForwardStart;\
    \
    while (true) {\
        for (int blockSide = blockSideStart; blockSide <= blockSideEnd; blockSide++) {\
            glm::ivec2 blockPosition = { BLOCK_X, BLOCK_Y };\
            \
            BlockData *blockData = BlockChunk::getSampleBlock(blockSample, blockPosition);\
            \
            if (blockData == nullptr) {\
                continue;\
            }\
            \
            int blockIndex = blockData->frontIndex;\
            entt::entity block = world.blocks.getBlock(blockIndex);\
            \
            /* Check collision component */\
            if (!world.blocks.registry.all_of<BlockCollisionComponent>(block)) {\
                continue;\
            }\
            \
            AXIS_BLOCK_CHECK\
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
    position.AXIS = endPosition;

bool BodySystem::getCollisionX(const BodyMovement &movement, float &endPosition) {
    GET_COLLISION_AXIS(x, y)
}

bool BodySystem::getCollisionY(const BodyMovement &movement, float &endPosition) {
    GET_COLLISION_AXIS(y, x)
}

void BodySystem::moveX(World &world, glm::vec2 &position, VelocityComponent &velocity, BodyComponent &body) {
    MOVE_AXIS(
        x, y,
        blockForwardStart,
        blockForwardEnd,
        blockForward,
        blockSide,
        getCollisionX, 
        if (world.blocks.registry.all_of<OneWayBlockComponent>(block)) {\
            continue;\
        })

    if (collided) {
        velocity.velocity.x = velocity.oldVelocity.x = 0.0f;
    }
}

void BodySystem::moveY(World &world, glm::vec2 &position, VelocityComponent &velocity, BodyComponent &body) {
    body.isOnFloor = false;
    body.isOnCeiling = false;

    MOVE_AXIS(
        y, x,
        blockSideStart,
        blockSideEnd,
        blockSide,
        blockForward,
        getCollisionY,
        /* Check one-way collision */\
        if (world.blocks.registry.all_of<OneWayBlockComponent>(block)) {\
            if (velocity.velocity.y <= 0.0f || blockPosition.y < position.y + body.size.y) {\
                continue;\
            }\
        })

    if (collided) {
        if (velocity.velocity.y < 0.0f) {
            body.isOnCeiling = true;
        }
        else {
            body.isOnFloor = true;
        }

        velocity.velocity.y = velocity.oldVelocity.y = 0.0f;
    }
}

void BodySystem::update(World &world) {
    auto view = world.entities.registry.view<PositionComponent, VelocityComponent, BodyComponent>();

    for (auto [entity, position, velocity, body] : view.each()) {
        moveY(world, position.position, velocity, body);
        moveX(world, position.position, velocity, body);

        velocity.oldVelocity = velocity.velocity;
    }
}