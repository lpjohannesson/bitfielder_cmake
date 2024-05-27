#include "server.h"
#include <iostream>
#include "world/entity/components/id_component.h"
#include "world/entity/components/position_component.h"

using namespace bf;

void Server::addClient(ClientConnection *client) {
    // Create new player object
    client->player = world.entities.spawnEntity();
    world.content.createPlayer(client->player, world, { 0.0f, 0.0f });

    // Send new player between clients
    for (ClientConnection *otherClient : clients) {
        writeRemotePlayer(client, otherClient->player);
        writeRemotePlayer(otherClient, client->player);
    }

    clients.push_back(client);

    // Load chunks
    writeChunk(client, world.map.getChunk(0));
    writeChunk(client, world.map.getChunk(1));
}

void Server::removeClient(ClientConnection *client) {
    entt::registry &entityRegistry = world.entities.registry;

    // Destroy entity, keep ID
    int playerID = entityRegistry.get<IDComponent>(client->player).id;
    world.entities.despawnEntity(playerID);

    // Erase from client list
    clients.erase(std::remove(clients.begin(), clients.end(), client));

    // Send disconnection to others
    for (ClientConnection *otherClient : clients) {
        writeDespawnRemotePlayer(otherClient, playerID);
    }
}

void Server::writeChunk(ClientConnection *client, BlockChunk *chunk) {
    Packet packet;

    packet << 0 << chunk->getMapIndex();

    // TODO: Chunk compression, host to network data, private chunk data

    // Chunk data
    packet.write((char*)&chunk->data, sizeof(BlockChunk::data));

    client->writePacket(packet);
}

void Server::writeEntityPosition(ClientConnection *client, entt::entity entity) {
    // TODO: Use separate packet for position

    Packet packet;

    entt::registry &entityRegistry = world.entities.registry;

    int entityID = entityRegistry.get<IDComponent>(entity).id;
    glm::vec2 entityPosition = entityRegistry.get<PositionComponent>(entity).position;

    packet << 3 << entityID << entityPosition;

    client->writePacket(packet);
}

void Server::writeRemotePlayer(ClientConnection *client, entt::entity player) {
    Packet packet;

    entt::registry &entityRegistry = world.entities.registry;

    int playerID = entityRegistry.get<IDComponent>(player).id;
    glm::vec2 playerPosition = entityRegistry.get<PositionComponent>(player).position;

    packet << 1 << playerID << playerPosition;

    client->writePacket(packet);
}

void Server::writeDespawnRemotePlayer(ClientConnection *client, int playerID) {
    Packet packet;

    packet << 2 << playerID;

    client->writePacket(packet);
}

void Server::readPacket(ClientConnection *client, Packet &packet) {
    entt::registry &entityRegistry = world.entities.registry;

    // TODO: Ensure entity has component
    int playerID = entityRegistry.get<IDComponent>(client->player).id;
    PositionComponent &playerPosition = entityRegistry.get<PositionComponent>(client->player);

    packet >> playerPosition.position;

    // TODO: Make timed polls for positions
    for (ClientConnection *otherClient : clients) {
        if (client == otherClient) {
            continue;
        }

        writeEntityPosition(otherClient, client->player);
    }
}

Server::Server() {
    BlockChunk &chunk = world.map.createChunk(0);
    mapGenerator.generateChunk(world, chunk);

	BlockChunk &chunk2 = world.map.createChunk(1);
    mapGenerator.generateChunk(world, chunk2);
}