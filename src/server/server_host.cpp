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
                std::cout << "Player connected." << std::endl;
                
                ClientConnection* client = new RemoteClientConnection(event.peer);
                clients.emplace(event.peer, client);

                server.addClient(client);

                break;
            }
            
            case ENET_EVENT_TYPE_DISCONNECT: {
                std::cout << "Player disconnected." << std::endl;

                delete clients.at(event.peer);
                clients.erase(event.peer);

                break;
            }
        
            case ENET_EVENT_TYPE_RECEIVE: {
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