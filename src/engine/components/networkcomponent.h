#ifndef NETWORKCOMPONENT_H
#define NETWORKCOMPONENT_H

#include "Component.h"
#include "raknet/RakPeerInterface.h"
#include "glm/glm.hpp"

class NetworkComponent :
        public Component
{
public:
    NetworkComponent(std::shared_ptr<GameObject> parent, RakNet::NetworkID networkId);
    ~NetworkComponent();
    RakNet::NetworkID getNetworkID();
    bool trySetTransform();
    void setNetworkID(RakNet::NetworkID netID);
private:
    RakNet::NetworkID m_netID;
    glm::vec3 m_pos, m_rot;

    bool compareVec(glm::vec3 v1, glm::vec3 v2);
};

#endif // NETWORKCOMPONENT_H
