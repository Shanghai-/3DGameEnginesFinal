#ifndef NETWORKSYSTEM_H
#define NETWORKSYSTEM_H

#include "System.h"
#include "raknet/RakPeerInterface.h"

#include "engine/components/networkcomponent.h"

using namespace RakNet;

class NetworkSystem : public System
{
public:
    NetworkSystem(int priority, bool isServer);
    ~NetworkSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);

    void tick(float seconds);
private:
    RakPeerInterface* m_peer;
    bool m_isServer;

    QSet<std::shared_ptr<NetworkComponent>> m_network;
};

#endif // NETWORKSYSTEM_H
