#include "server_host.h"
#include <iostream>
#include <chrono>
#include <future>
#include <string>
#include <SDL2/SDL.h>

using namespace bf;

std::string ServerHost::getCin() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void ServerHost::disconnectClient(RemoteClientConnection *client) {
    enet_peer_disconnect(client->networkPeer, 0);

    ENetEvent event;
    bool finished = false;

    while (enet_host_service(networkHost, &event, 3000) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy(event.packet);
            break;
    
        case ENET_EVENT_TYPE_DISCONNECT:
            finished = true;
            break;
        }

        if (finished) {
            break;
        }
    }

    enet_peer_reset(client->networkPeer);
}

ServerHost::ServerHost(int port) {
    SDL_Init(SDL_INIT_TIMER);

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

    auto future = std::async(std::launch::async, getCin);

    while (true) {
        if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            std::string input = future.get();
            future = std::async(std::launch::async, getCin);

            if (input == "stop") {
                return;
            }
        }

        ENetEvent event;

        while (enet_host_service(networkHost, &event, 100) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                // Add client
                RemoteClientConnection *client = new RemoteClientConnection();
                remoteClients.push_back(client);

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
                remoteClients.erase(std::remove(remoteClients.begin(), remoteClients.end(), client));

                server.removeClient(client);
                delete client;

                std::cout << "Player disconnected. (Player count: " << server.clients.size() << ")" << std::endl;

                break;
            }
        
            case ENET_EVENT_TYPE_RECEIVE: {
                RemoteClientConnection *client = (RemoteClientConnection*)event.peer->data;

                // Read packet
                Packet packet;
                packet.write((char*)event.packet->data, (int)event.packet->dataLength);

                server.readPacket(client, packet);

                enet_packet_destroy(event.packet);
                break;
            }
            }
        }
    }
}

ServerHost::~ServerHost() {
    std::cout << "Stopping server..." << std::endl;
    
    for (RemoteClientConnection *client : remoteClients) {
        disconnectClient(client);
    }

    enet_host_destroy(networkHost);
    enet_deinitialize();
}