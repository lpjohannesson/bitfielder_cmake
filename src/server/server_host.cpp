#include "server_host.h"
#include <iostream>
#include "remote_client_connection.h"

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

                // Link peer with client
                client->networkPeer = event.peer;
                event.peer->data = client;

                server.addClient(client);

                std::cout << "Player connected. (Player count: " << server.clients.size() << ")" << std::endl;

                break;
            }
            
            case ENET_EVENT_TYPE_DISCONNECT: {
                // Remove client
                RemoteClientConnection *client = (RemoteClientConnection*)event.peer->data;

                server.removeClient(client);
                delete client;

                std::cout << "Player disconnected. (Player count: " << server.clients.size() << ")" << std::endl;

                break;
            }
        
            case ENET_EVENT_TYPE_RECEIVE: {
                RemoteClientConnection *client = (RemoteClientConnection*)event.peer->data;;

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