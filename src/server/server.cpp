#include "server.h"
#include <iostream>
#include "core/packet_types.h"
#include "world/world_file.h"
#include "world/block/block_serializer.h"
#include "world/entity/components/id_component.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/animation_component.h"
#include "world/entity/components/flip_component.h"
#include "world/entity/components/aim_component.h"
#include "world/block/components/block_collision_component.h"
#include "world/block/block_sample.h"

using namespace bf;

void Server::broadcastPacket(Packet &packet, ClientConnection *client) {
    for (ClientConnection *otherClient : clients) {
        if (client == otherClient) {
            continue;
        }

        otherClient->writePacket(packet);
    }
}

void Server::addClient(ClientConnection *client) {
    // Create new player object
    client->player = world.entities.spawnEntity();
    world.content.entities.createPlayer(client->player, world);

    // Send world to new player
    Packet packet;

    for (ClientConnection *otherClient : clients) {
        writeRemotePlayer(packet, otherClient->player);
    }

    for (auto &chunkPair : world.map.chunks) {
        writeChunkData(packet, chunkPair.second);
    }

    // Find random spawn above solid block
    glm::ivec2 spawnPosition = { 50 + random.randomInt(random.randomEngine) % 50, -1 };

    BlockSample<BlockChunk> spawnSample;
    spawnSample.sampleBlocks(world.map, spawnPosition.x, spawnPosition.x);

    entt::registry &blocksRegistry = world.blocks.registry;

    while (true) {
        glm::ivec2 belowSpawnPosition = { spawnPosition.x, spawnPosition.y + 1 };
        BlockData *belowBlockData = BlockChunk::getSampleBlock(belowSpawnPosition, spawnSample);

        if (belowBlockData == nullptr) {
            break;
        }

        entt::entity belowBlock = world.blocks.getEntity(belowBlockData->getFrontIndex());

        if (blocksRegistry.all_of<BlockCollisionComponent>(belowBlock)) {
            break;
        }

        spawnPosition = belowSpawnPosition;
    }

    writeTeleportPlayer(packet, client, spawnPosition);

    clients.push_back(client);
    client->writePacket(packet);

    // Send new player to others
    Packet otherPacket;
    writeRemotePlayer(otherPacket, client->player);

    broadcastPacket(otherPacket, client);
}

void Server::removeClient(ClientConnection *client) {
    clients.erase(std::remove(clients.begin(), clients.end(), client));

    // Destroy entity, keep ID
    entt::registry &entityRegistry = world.entities.registry;

    int playerID = entityRegistry.get<IDComponent>(client->player).id;
    world.entities.despawnEntity(playerID);

    // Send disconnection to others
    Packet despawnPacket;
    writeDespawnEntity(despawnPacket, playerID);

    broadcastPacket(despawnPacket, client);
}

void Server::writePlayerState(Packet &packet, entt::entity player) {
    writeEntityPosition(packet, player);
    writeEntityAnimation(packet, player);
    writeEntityFlip(packet, player);
    writeEntityAim(packet, player);
}

void Server::writeTeleportPlayer(Packet &packet, ClientConnection *client, glm::vec2 position) {
    packet << (int)ServerPacket::TELEPORT_PLAYER << position;

    world.entities.registry.get<PositionComponent>(client->player).position = position;
}

void Server::writeChunkData(Packet &packet, BlockChunk &chunk) {
    packet << (int)ServerPacket::CHUNK_DATA << chunk.getMapIndex();

    BlockSerializer::writeChunk(chunk, world, packet);
}

void Server::writeReplaceBlock(Packet &packet, glm::ivec2 position, bool onFrontLayer, int blockIndex) {
    packet << (int)ServerPacket::REPLACE_BLOCK << position << onFrontLayer << blockIndex;
}

void Server::writeDespawnEntity(Packet &packet, int entityID) {
    packet << (int)ServerPacket::DESPAWN_ENTITY << entityID;
}

void Server::writeEntityPosition(Packet &packet, entt::entity entity) {
    entt::registry &entityRegistry = world.entities.registry;
    
    int entityID = entityRegistry.get<IDComponent>(entity).id;
    glm::vec2 position = entityRegistry.get<PositionComponent>(entity).position;

    packet << (int)ServerPacket::ENTITY_POSITION << entityID << position;
}

void Server::writeEntityAnimation(Packet &packet, entt::entity entity) {
    entt::registry &entityRegistry = world.entities.registry;

    int entityID = entityRegistry.get<IDComponent>(entity).id;
    AnimationComponent &animation = entityRegistry.get<AnimationComponent>(entity);

    packet << (int)ServerPacket::ENTITY_ANIMATION << entityID << animation.index;
}

void Server::writeEntityFlip(Packet &packet, entt::entity entity) {
    entt::registry &entityRegistry = world.entities.registry;

    int entityID = entityRegistry.get<IDComponent>(entity).id;
    FlipComponent &flip = entityRegistry.get<FlipComponent>(entity);

    packet << (int)ServerPacket::ENTITY_FLIP << entityID << flip.flipX;
}

void Server::writeEntityAim(Packet &packet, entt::entity entity) {
    entt::registry &entityRegistry = world.entities.registry;
    
    int entityID = entityRegistry.get<IDComponent>(entity).id;
    AimComponent &aim = entityRegistry.get<AimComponent>(entity);

    packet << (int)ServerPacket::ENTITY_AIM << entityID << aim.aim;
}

void Server::writeRemotePlayer(Packet &packet, entt::entity player) {
    // TODO: Spawn entity on client after state recieved
    entt::registry &entityRegistry = world.entities.registry;
    int entityID = entityRegistry.get<IDComponent>(player).id;

    packet << (int)ServerPacket::REMOTE_PLAYER << entityID;
    
    writePlayerState(packet, player);
}

void Server::readPlayerState(ClientConnection *client, Packet &packet) {
    // TODO: Rubber-banding
    entt::registry &entityRegistry = world.entities.registry;
    
    PositionComponent &position = entityRegistry.get<PositionComponent>(client->player);
    AnimationComponent &animation = entityRegistry.get<AnimationComponent>(client->player);
    FlipComponent &flip = entityRegistry.get<FlipComponent>(client->player);
    AimComponent &aim = entityRegistry.get<AimComponent>(client->player);

    packet >> position.position >> animation.index >> flip.flipX >> aim.aim;

    // TODO: Make timed polls
    Packet castPacket;
    writePlayerState(castPacket, client->player);

    broadcastPacket(castPacket, client);
}

void Server::readReplaceBlock(ClientConnection *client, Packet &packet) {
    // TODO: Validity checks
    glm::ivec2 position;
    bool onFrontLayer;
    int blockIndex;

    packet >> position >> onFrontLayer >> blockIndex;

    BlockData *blockData = BlockChunk::getWorldBlock(position, world.map);

    if (blockData == nullptr) {
        return;
    }

    if (onFrontLayer) {
        blockData->setFrontIndex(blockIndex);
    }
    else {
        blockData->setBackIndex(blockIndex);
    }

    Box2i box;
    world.updateBlock(position, box);

    Packet castPacket;
    writeReplaceBlock(castPacket, position, onFrontLayer, blockIndex);

    broadcastPacket(castPacket, client);
}

void Server::readPacket(ClientConnection *client, Packet &packet) {
    // TODO: AFK checks
    int packetID;
	packet >> packetID;

	switch ((ClientPacket)packetID) {
	case ClientPacket::PLAYER_STATE:
        readPlayerState(client, packet);
        break;

    case ClientPacket::REPLACE_BLOCK:
        readReplaceBlock(client, packet);
        break;
	}
}

Server::Server() {
    if (!WorldFile::loadWorld(world, "world")) {
        // Generate world
        for (int i = 0; i < 10; i++) {
            BlockChunk &chunk = world.map.createChunk(i);
            mapGenerator.generateChunk(chunk, world);
        }
    }
}

Server::~Server() {
    WorldFile::saveWorld(world, "world");
}