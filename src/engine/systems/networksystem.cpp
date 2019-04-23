#include "networksystem.h"

NetworkSystem::NetworkSystem(int priority) : System(priority)
{
    auto localPort = SocketDescriptor(4567, 0);
    //RakPeerInterface::Startup(10, So)
    m_peer = RakPeerInterface::GetInstance();
}

NetworkSystem::~NetworkSystem() {
}
