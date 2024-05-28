#include "remote_world_scene.h"
#include <iostream>

using namespace bf;

void RemoteWorldScene::update() {
    ENetEvent event;

    while (enet_host_service(networkHost, &event, 1) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_DISCONNECT: {
            // TODO: Server disconnect
        }
    
        case ENET_EVENT_TYPE_RECEIVE: {
            Packet packet;
            packet.write((char*)event.packet->data, event.packet->dataLength);
            readPacket(packet);

            enet_packet_destroy(event.packet);

            break;
        }
        }
    }

    WorldScene::update();
}

RemoteWorldScene::RemoteWorldScene(const char *ip, int port) {
    // Start network
    if (enet_initialize() != 0) {
        std::cout << "Network failed." << std::endl;
    }
    
    networkHost = enet_host_create(NULL, 1, 2, 0, 0);

    if (networkHost == NULL) {
        std::cout << "Network host failed." << std::endl;
    }

    // Connect to server
    std::cout << "Connecting to server..." << std::endl;

    enet_address_set_host(&networkAddress, ip);
    networkAddress.port = port;

    networkServer = enet_host_connect(networkHost, &networkAddress, 2, 0);

    ENetEvent event;

    if (enet_host_service(networkHost, &event, 5000) == 0 ||
        event.type != ENET_EVENT_TYPE_CONNECT) {

        // TODO: Network fail logic
        std::cout << "Failed." << std::endl;
        enet_peer_reset(networkServer);

        return;
    }

    std::cout << "Connected." << std::endl;

    // Create server connection
    server = &remoteServerConnection;
    remoteServerConnection.networkPeer = networkServer;
}

RemoteWorldScene::~RemoteWorldScene() {
    // Disconnect
    std::cout << "Disconnecting..." << std::endl;

    enet_peer_disconnect(networkServer, 0);

    ENetEvent event;

    while (enet_host_service(networkHost, &event, 3000) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy (event.packet);
            break;
    
        case ENET_EVENT_TYPE_DISCONNECT:
            std::cout << "Disconnected." << std::endl;
            return;
        }
    }

    enet_host_destroy(networkHost);
    enet_deinitialize();
}