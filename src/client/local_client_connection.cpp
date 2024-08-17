#include "local_client_connection.h"
#include "client_packet_manager.h"

using namespace bf;

void LocalClientConnection::writePacket(Packet &packet) {
    // Pass packet to world scene
    ClientPacketManager::readPacket(packet, *scene);
}