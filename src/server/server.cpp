#include "server.h"

using namespace bf;

void Server::addClient(ClientConnection *client) {
    clients.push_back(client);

    sendChunk(client, world.map.getChunk(0));
}

void Server::removeClient(ClientConnection *client) {
    std::remove(clients.begin(), clients.end(), client);
}

void Server::sendChunk(ClientConnection *client, BlockChunk *chunk) {
    Packet packet;

    // Chunk index
    packet << chunk->getMapIndex();

    // Chunk data
    packet.write((char*)&chunk->data, sizeof(BlockChunk::data));

    client->writePacket(packet);
}

Server::Server() {
    BlockChunk &chunk = world.map.createChunk(0);
    mapGenerator.generateChunk(world, chunk);

	BlockChunk &chunk2 = world.map.createChunk(2);
    mapGenerator.generateChunk(world, chunk2);
}