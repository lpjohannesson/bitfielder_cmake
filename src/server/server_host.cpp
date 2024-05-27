#include "server_host.h"
#include <iostream>

using namespace bf;

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
                RemoteClientConnection *client = new RemoteClientConnection();

                // Attach peer
                client->networkPeer = event.peer;

                // Link peer with client
                event.peer->data = client;

                server.addClient(client);

                std::cout << "Player connected. (Player count: " << server.clients.size() << ")" << std::endl;

                break;
            }
            
            case ENET_EVENT_TYPE_DISCONNECT: {
                ClientConnection *client = (ClientConnection*)event.peer->data;

                // Remove client
                server.removeClient(client);

                delete client;

                std::cout << "Player disconnected. (Player count: " << server.clients.size() << ")" << std::endl;

                break;
            }
        
            case ENET_EVENT_TYPE_RECEIVE: {
                ClientConnection *client = (ClientConnection*)event.peer->data;;

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