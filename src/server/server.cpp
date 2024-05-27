#include "server.h"
#include <iostream>
#include "world/entity/components/id_component.h"

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

    // Send disconnect packet to others
    for (ClientConnection *otherClient : clients) {
        writeDespawnRemotePlayer(otherClient, playerID);
    }
}

void Server::writeChunk(ClientConnection *client, BlockChunk *chunk) {
    Packet packet;

    // Packet ID, chunk index
    packet << 0 << chunk->getMapIndex();

    // TODO: Chunk compression, host to network data, private chunk data

    // Chunk data
    packet.write((char*)&chunk->data, sizeof(BlockChunk::data));

    client->writePacket(packet);
}

void Server::writeRemotePlayer(ClientConnection *client, entt::entity player) {
    Packet packet;

    entt::registry &entityRegistry = world.entities.registry;

    // Packet ID, player ID
    int playerID = entityRegistry.get<IDComponent>(player).id;
    packet << 1 << playerID;

    client->writePacket(packet);
}

void Server::writeDespawnRemotePlayer(ClientConnection *client, int playerID) {
    Packet packet;

    packet << 2 << playerID;

    client->writePacket(packet);
}

void Server::readPacket(ClientConnection *client, Packet &packet) {
    int test;
    packet >> test;

    std::cout << test << std::endl;
}

Server::Server() {
    BlockChunk &chunk = world.map.createChunk(0);
    mapGenerator.generateChunk(world, chunk);

	BlockChunk &chunk2 = world.map.createChunk(1);
    mapGenerator.generateChunk(world, chunk2);
}