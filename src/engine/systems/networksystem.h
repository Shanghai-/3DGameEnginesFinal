#ifndef NETWORKSYSTEM_H
#define NETWORKSYSTEM_H

#include "System.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/NetworkIDObject.h"
#include "raknet/NetworkIDManager.h"
#include "engine/components/networkcomponent.h"

class GameWorld;

using namespace RakNet;

class PlayerObject : public NetworkIDObject {};

class NetworkSystem : public System
{
public:
    NetworkSystem(int priority, std::shared_ptr<GameWorld> gameworld, bool isServer);
    ~NetworkSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);

    void tick(float seconds);
    void SetPlayer(std::shared_ptr<NetworkComponent> comp);
private:
    RakPeerInterface *m_peer;
    bool m_isServer;
    NetworkIDManager m_networkIDManager;
    QSet<std::shared_ptr<NetworkComponent>> m_network;
    std::shared_ptr<GameWorld> m_gw;
    std::shared_ptr<NetworkComponent> m_player;
};

#endif // NETWORKSYSTEM_H
