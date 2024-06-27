#include "local_client_connection.h"

using namespace bf;

void LocalClientConnection::writePacket(Packet &packet) {
    // Pass packet to world scene
    scene->readPacket(packet);
}