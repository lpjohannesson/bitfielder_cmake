#include "server_host.h"
#include <iostream>

using namespace bf;

ClientConnection *ServerHost::getRemoteClient(ENetPeer *peer) {
    auto foundClient = peerClients.find(peer);

    if (foundClient == peerClients.end()) {
        return nullptr;
    }

    return foundClient->second;
}

ServerHost::ServerHost(int port) {
    // Start network
    if (enet_initialize() != 0) {
        std::cout << "Network failed." << std::endl;
    }

    networkAddress.host = ENET_HOST_ANY;
    networkAddress.port = port;

    networkHost = enet_host_create(&networkAddress, 32, 2, 0, 0);

    if (networkHost == NULL) {
        std::cout << "Network host failed." << std::endl;
        return;
    }

    std::cout << "Server started on port " << port << "." << std::endl;

    while (true) {
        ENetEvent event;

        while (enet_host_service(networkHost, &event, 100) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                // Add client
                remoteClients.emplace_back();
                RemoteClientConnection &client = remoteClients.back();

                client.networkPeer = event.peer;

                peerClients.emplace(event.peer, &client);

                server.addClient(&client);

                std::cout << "Player connected. (Player count: " << server.clients.size() << ")" << std::endl;

                break;
            }
            
            case ENET_EVENT_TYPE_DISCONNECT: {
                // Find client or skip
                ClientConnection *client = getRemoteClient(event.peer);

                if (client == nullptr) {
                    break;
                }

                // Remove client
                server.removeClient(client);

                remoteClients.erase(std::remove_if(remoteClients.begin(), remoteClients.end(),
                    [client](RemoteClientConnection &remoteClient) { return &remoteClient == client; }));

                peerClients.erase(event.peer);

                std::cout << "Player disconnected. (Player count: " << server.clients.size() << ")" << std::endl;

                break;
            }
        
            case ENET_EVENT_TYPE_RECEIVE: {
                // Find client or skip
                ClientConnection *client = getRemoteClient(event.peer);

                if (client == nullptr) {
                    break;
                }

                // Read packet
                Packet packet;
                packet.write((char*)event.packet->data, event.packet->dataLength);

                server.readPacket(client, packet);

                enet_packet_destroy(event.packet);
                break;
            }
            }
        }
    }
}

ServerHost::~ServerHost() {
    enet_host_destroy(networkHost);
    enet_deinitialize();
}