#include "server.h"
#include <iostream>
#include "core/packet_types.h"
#include "world/entity/components/id_component.h"
#include "world/entity/components/position_component.h"
#include "world/entity/components/sprite_animation_component.h"
#include "world/entity/components/sprite_flip_component.h"

using namespace bf;

void Server::addClient(ClientConnection *client) {
    // Create new player object
    client->player = world.entities.spawnEntity();

    PlayerSpawnProperties spawnProperties;
    world.content.createPlayer(client->player, world, spawnProperties);

    // Send new player between clients
    for (ClientConnection *otherClient : clients) {
        writeRemotePlayer(client, otherClient->player);
        writeRemotePlayer(otherClient, client->player);
    }

    clients.push_back(client);

    // Load chunks
    for (int i = 0; i < 10; i++) {
        writeBlockChunk(client, world.map.getChunk(i));
    }
}

void Server::removeClient(ClientConnection *client) {
    // Destroy entity, keep ID
    int playerID = entityRegistry->get<IDComponent>(client->player).id;
    world.entities.despawnEntity(playerID);

    // Erase from client list
    clients.erase(std::remove(clients.begin(), clients.end(), client));

    // Send disconnection to others
    for (ClientConnection *otherClient : clients) {
        writeDespawnEntity(otherClient, playerID);
    }
}

void Server::writeBlockChunk(ClientConnection *client, BlockChunk *chunk) {
    Packet packet;

    packet << (int)ServerPacket::BLOCK_CHUNK << chunk->getMapIndex();

    // TODO: Chunk compression, host to network data, private chunk data

    // Chunk data
    packet.write((char*)&chunk->data, sizeof(BlockChunk::data));

    client->writePacket(packet);
}

void Server::writeDespawnEntity(ClientConnection *client, int entityID) {
    Packet packet;

    packet << (int)ServerPacket::DESPAWN_ENTITY << entityID;

    client->writePacket(packet);
}

void Server::writeEntityPosition(ClientConnection *client, entt::entity entity) {
    Packet packet;

    int entityID = entityRegistry->get<IDComponent>(entity).id;
    glm::vec2 position = entityRegistry->get<PositionComponent>(entity).position;

    packet << (int)ServerPacket::ENTITY_POSITION << entityID << position;

    client->writePacket(packet);
}

void Server::writeEntitySpriteAnimation(ClientConnection *client, entt::entity entity) {
    Packet packet;

    int entityID = entityRegistry->get<IDComponent>(entity).id;
    SpriteAnimationComponent &animation = entityRegistry->get<SpriteAnimationComponent>(entity);

    packet << (int)ServerPacket::ENTITY_SPRITE_ANIMATION << entityID << animation.animationIndex;

    client->writePacket(packet);
}

void Server::writeEntitySpriteFlip(ClientConnection *client, entt::entity entity) {
    Packet packet;

    int entityID = entityRegistry->get<IDComponent>(entity).id;
    SpriteFlipComponent &spriteFlip = entityRegistry->get<SpriteFlipComponent>(entity);

    packet << (int)ServerPacket::ENTITY_SPRITE_FLIP << entityID << spriteFlip.flipX;

    client->writePacket(packet);
}

void Server::writeRemotePlayer(ClientConnection *client, entt::entity player) {
    // TODO: Sprite info
    Packet packet;

    int entityID = entityRegistry->get<IDComponent>(player).id;
    glm::vec2 position = entityRegistry->get<PositionComponent>(player).position;
    SpriteAnimationComponent &spriteAnimation = entityRegistry->get<SpriteAnimationComponent>(player);
    SpriteFlipComponent &spriteFlip = entityRegistry->get<SpriteFlipComponent>(player);

    packet << (int)ServerPacket::REMOTE_PLAYER << entityID << position << spriteAnimation.animationIndex << spriteFlip.flipX;

    client->writePacket(packet);
}

void Server::readPlayerPosition(ClientConnection *client, Packet &packet) {
    // TODO: Rubber-banding
    PositionComponent &position = entityRegistry->get<PositionComponent>(client->player);
    packet >> position.position;

    // TODO: Make timed polls
    for (ClientConnection *otherClient : clients) {
        if (client == otherClient) {
            continue;
        }

        writeEntityPosition(otherClient, client->player);
    }
}

void Server::readPlayerSpriteAnimation(ClientConnection *client, Packet &packet) {
    SpriteAnimationComponent &spriteAnimation = entityRegistry->get<SpriteAnimationComponent>(client->player);
    packet >> spriteAnimation.animationIndex;

    // TODO: Make timed polls
    for (ClientConnection *otherClient : clients) {
        if (client == otherClient) {
            continue;
        }

        writeEntitySpriteAnimation(otherClient, client->player);
    }
}

void Server::readPlayerSpriteFlip(ClientConnection *client, Packet &packet) {
    SpriteFlipComponent &spriteFlip = entityRegistry->get<SpriteFlipComponent>(client->player);
    packet >> spriteFlip.flipX;

    // TODO: Make timed polls
    for (ClientConnection *otherClient : clients) {
        if (client == otherClient) {
            continue;
        }

        writeEntitySpriteFlip(otherClient, client->player);
    }
}

void Server::readPacket(ClientConnection *client, Packet &packet) {
    int packetID;
	packet >> packetID;

	switch ((ClientPacket)packetID) {
	case ClientPacket::PLAYER_POSITION:
        readPlayerPosition(client, packet);
        break;

    case ClientPacket::PLAYER_SPRITE_ANIMATION:
        readPlayerSpriteAnimation(client, packet);
        break;

    case ClientPacket::PLAYER_SPRITE_FLIP:
        readPlayerSpriteFlip(client, packet);
        break;
	}
}

Server::Server() {
    entityRegistry = &world.entities.registry;

    for (int i = 0; i < 10; i++) {
        BlockChunk &chunk = world.map.createChunk(i);
        mapGenerator.generateChunk(world, chunk);
    }
}