#include "server.h"

using namespace bf;

void Server::addClient(ClientConnection *client) {
    clients.push_back(client);

    // Test packet
    Packet testPacket;
    testPacket >> 2024 >> 1234;

    client->writePacket(testPacket);
}

Server::Server() {
    BlockChunk &chunk = world.map.createChunk(0);
    mapGenerator.generateChunk(world, chunk);

	BlockChunk &chunk2 = world.map.createChunk(2);
    mapGenerator.generateChunk(world, chunk2);
}