#include "remote_client_connection.h"

using namespace bf;

void RemoteClientConnection::writePacket(Packet &packet) {
    ENetPacket *networkPacket = enet_packet_create(packet.data.data(), packet.data.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(networkPeer, 0, networkPacket);
}

RemoteClientConnection::RemoteClientConnection(ENetPeer *networkPeer) {
    this->networkPeer = networkPeer;
}