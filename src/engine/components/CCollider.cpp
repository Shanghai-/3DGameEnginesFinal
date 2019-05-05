#include "CCollider.h"
#include "CTransform.h"

CCollider::CCollider(std::shared_ptr<GameObject> parent,
                     std::shared_ptr<CollisionVolume> vol,
                     bool isTrigger) :
    CCollider(parent, vol, isTrigger, nullptr, COLLISION_LAYERS::DEFAULT)
{
}



CCollider::CCollider(std::shared_ptr<GameObject> parent,
                     std::shared_ptr<CollisionVolume> vol,
                     bool isTrigger,
                     std::shared_ptr<CollisionResponse> resp) :
    CCollider(parent, vol, isTrigger, resp, COLLISION_LAYERS::DEFAULT)
{
}

CCollider::CCollider(std::shared_ptr<GameObject> parent,
                     std::shared_ptr<CollisionVolume> vol,
                     bool isTrigger,
                     std::shared_ptr<CollisionResponse> resp,
                     int collision_layer) :
    Component(parent),
    m_collVol(vol),
    m_isTrigger(isTrigger),
    m_collResp(resp),
    m_collisionLayer(collision_layer)
{
    m_collVol->setOwner(this);
}

CCollider::~CCollider()
{
}

std::shared_ptr<CollisionVolume> CCollider::getCollisionVolume()
{
    return m_collVol;
}

bool CCollider::isTrigger() const
{
    return m_isTrigger;
}

void CCollider::onCollide(std::shared_ptr<GameObject> other)
{
    if (m_collResp != nullptr) {
        m_collResp->onCollide(other);
    }
}

void CCollider::onCollisionContinue(std::shared_ptr<GameObject> other)
{
    if (m_collResp != nullptr) {
        m_collResp->onCollisionContinue(other);
    }
}

void CCollider::onCollisionEnd(std::shared_ptr<GameObject> other)
{
    if (m_collResp != nullptr) {
        m_collResp->onCollisionEnd(other);
    }
}

float CCollider::intersect(glm::vec3 origin, glm::vec3 direction)
{
    return m_collVol->intersect(origin, direction);
}

int CCollider::getLayer()
{
    return m_collisionLayer;
}

bool CCollider::ignores(int layer)
{
    return m_ignoredLayers.contains(layer);
}

void CCollider::ignoreLayer(int layer)
{
    m_ignoredLayers.insert(layer);
}

void CCollider::unignoreLayer(int layer)
{
    m_ignoredLayers.remove(layer);
}
