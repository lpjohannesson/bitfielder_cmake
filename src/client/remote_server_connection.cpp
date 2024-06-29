#include "remote_server_connection.h"
#include "scenes/menu_scene.h"

#ifdef NX
#include <switch.h>
#endif

using namespace bf;

bool RemoteServerConnection::startConnection(const char *ip, int port, std::string &error) {
    // Start network
#ifdef NX
    socketInitializeDefault();
#endif
    
    if (enet_initialize() != 0) {
        error = "Network failed.";
        return false;
    }
    
    networkHost = enet_host_create(NULL, 1, 2, 0, 0);

    if (networkHost == nullptr) {
        error = "Network host failed.";
        return false;
    }

    enet_address_set_host(&networkAddress, ip);
    networkAddress.port = port;

    // Create server connection
    networkPeer = enet_host_connect(networkHost, &networkAddress, 2, 0);

    return true;
}

void RemoteServerConnection::endConnection() {
    if (networkPeer != nullptr) {
        enet_peer_reset(networkPeer);
    }

#ifdef NX
    socketExit();
#endif
}

bool RemoteServerConnection::updateConnection() {
    ENetEvent event;

    while (enet_host_service(networkHost, &event, 1) > 0) {
        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            return true;
        }
    }

    return false;
}

bool RemoteServerConnection::host(WorldScene &scene) {
    // Always update world, even when paused
    scene.world.update();

    ENetEvent event;

    while (enet_host_service(networkHost, &event, 1) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_DISCONNECT: {
            // Show disconnection menu
            MenuScene *menuScene = new MenuScene();
            menuScene->changeState(MenuState::DISCONNECTED);

            engine->changeScene(menuScene);

            return false;
        }
    
        case ENET_EVENT_TYPE_RECEIVE: {
            Packet packet;
            packet.write((char*)event.packet->data, (int)event.packet->dataLength);
            scene.readPacket(packet);

            enet_packet_destroy(event.packet);

            break;
        }
        }
    }

    return true;
}

void RemoteServerConnection::writePacket(Packet &packet) {
    ENetPacket *networkPacket = enet_packet_create(packet.data.data(), packet.data.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(networkPeer, 0, networkPacket);
}

void RemoteServerConnection::end() {
    enet_peer_disconnect(networkPeer, 0);

    ENetEvent event;

    while (enet_host_service(networkHost, &event, 3000) > 0) {
        bool finished = false;

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

    enet_host_destroy(networkHost);
    enet_deinitialize();
}