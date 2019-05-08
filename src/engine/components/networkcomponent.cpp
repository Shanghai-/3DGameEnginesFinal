#include "networkcomponent.h"
#include "engine/components/CTransform.h"
#include "iostream"
#define EPSILON 0.00001f

NetworkComponent::NetworkComponent(std::shared_ptr<GameObject> parent, RakNet::NetworkID networkId) : Component(parent),
    m_netID(networkId)
{
}

NetworkComponent::~NetworkComponent() {

}

RakNet::NetworkID NetworkComponent::getNetworkID() {
    return m_netID;
}

void NetworkComponent::setNetworkID(RakNet::NetworkID netID) {
    m_netID = netID;
}

// Returns true and sets transform to new transform if the new transform differs by a nontrivial amount.
// For the sake of reducing the amount of packets being sent
bool NetworkComponent::trySetTransform() {
    auto trans = getSibling<CTransform>();
    auto pos = trans->pos;
    auto rot = trans->rot;
    if (compareVec(m_pos, pos) && compareVec(m_rot, rot)) {
        return false;
    } else {
        m_pos = pos;
        m_rot = rot;
        return true;
    }
}

bool NetworkComponent::compareVec(glm::vec3 v1, glm::vec3 v2) {
    if (std::abs(v1.x - v2.x + v1.y - v2.y + v1.z - v2.z) < EPSILON) {
        return true;
    }
    return false;
}
