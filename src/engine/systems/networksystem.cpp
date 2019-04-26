#include "networksystem.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

NetworkSystem::NetworkSystem(int priority, bool isServer) : System(priority),
    m_isServer(isServer)
{
    m_peer = RakPeerInterface::GetInstance();

    if (isServer) {
        SocketDescriptor sd(SERVER_PORT, 0);
        m_peer->Startup(MAX_CLIENTS, &sd, 1);
    } else {
        SocketDescriptor sd;
        m_peer->Startup(1, &sd, 1);
    }
}

NetworkSystem::~NetworkSystem() {
    RakPeerInterface::DestroyInstance(m_peer);
}

QString NetworkSystem::getComponentType() const
{
    return typeid(NetworkComponent).name();
}

inline uint qHash(const std::shared_ptr<NetworkComponent> &key) {
    return key->getId();
}


void NetworkSystem::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<NetworkComponent> netComp = std::dynamic_pointer_cast<NetworkComponent>(c);
    m_network.insert(netComp);
}

void NetworkSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<NetworkComponent> netComp = std::dynamic_pointer_cast<NetworkComponent>(c);
    m_network.remove(netComp);
}

void NetworkSystem::tick(float seconds)
{

}

