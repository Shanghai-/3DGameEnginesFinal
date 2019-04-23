#ifndef NETWORKSYSTEM_H
#define NETWORKSYSTEM_H

#include "System.h"
#include "raknet/RakPeerInterface.h"

using namespace RakNet;

class NetworkSystem : public System
{
public:
    NetworkSystem(int priority);
    ~NetworkSystem();

//    QString getComponentType() const;
//    void addComponent(const std::shared_ptr<Component> &c);
//    void removeComponent(const std::shared_ptr<Component> &c);

//    void tick(float seconds);
//    void draw();
private:
    RakPeerInterface* m_peer;
    bool m_isServer;
};

#endif // NETWORKSYSTEM_H
