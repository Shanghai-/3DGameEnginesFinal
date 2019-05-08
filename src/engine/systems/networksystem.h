#ifndef NETWORKSYSTEM_H
#define NETWORKSYSTEM_H

#include "System.h"
#include "raknet/NetworkIDObject.h"
#include "raknet/NetworkIDManager.h"
#include "engine/components/networkcomponent.h"
#include <QMap>

class GameWorld;

using namespace RakNet;

class PlayerObject : public NetworkIDObject {
public:
    std::shared_ptr<NetworkComponent> networkComponent;
};

class NetworkSystem : public System
{
public:
    NetworkSystem(int priority, GameWorld *gameworld, bool isServer);
    ~NetworkSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);

    void tick(float seconds);
    void setPlayer(std::shared_ptr<NetworkComponent> comp);
private:
    RakPeerInterface *m_peer;
    bool m_isServer;
    NetworkIDManager m_networkIDManager;
    QSet<std::shared_ptr<NetworkComponent>> m_networkObjects;
    GameWorld *m_gw;
    SystemAddress m_serverAddress;
    std::shared_ptr<NetworkComponent> m_player;
    void addPlayer(PlayerObject* playerObject, QString name, NetworkID netID);
    QSet<NetworkID> m_activeNetworkIDs;
    bool m_isInitialized;
    QMap<RakNet::SystemAddress, RakNet::NetworkID> m_playerAddresses;

    void removePlayer(RakNet::SystemAddress address);
};

#endif // NETWORKSYSTEM_H
