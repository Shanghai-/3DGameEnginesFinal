#include "CollisionSystem.h"

#include "engine/graphics/Graphics.h"

#include <iostream>

CollisionSystem::CollisionSystem(int priority) :
    System(priority)
{
    m_type = QString::fromStdString(typeid(CCollider).name());
}

CollisionSystem::~CollisionSystem()
{
}

QString CollisionSystem::getComponentType() const
{
    return m_type;
}

inline uint qHash(const std::shared_ptr<CCollider> &key) {
    return key->getId();
}

void CollisionSystem::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CCollider> coll = std::dynamic_pointer_cast<CCollider>(c);
    m_colliders.insert(coll);
    //m_colliders.append(coll);
}

void CollisionSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CCollider> coll = std::dynamic_pointer_cast<CCollider>(c);
    //std::cout << "removed " << m_colliders.removeAll(coll) << " colliders." << std::endl;
    //m_colliders.remove(m_colliders.indexOf(coll));
    m_colliders.remove(coll);
}

void CollisionSystem::tick(float seconds)
{
    QSet<std::shared_ptr<CCollider>> resolved;

    QSetIterator<std::shared_ptr<CCollider>> outer(m_colliders);
    while (outer.hasNext())
    {
        std::shared_ptr<CCollider> thisColl = outer.next();

        QSetIterator<std::shared_ptr<CCollider>> inner(m_colliders);
        while (inner.hasNext())
        {
            std::shared_ptr<CCollider> other = inner.next();

            // Ignore colliders that have already been resolved
            if (resolved.contains(other)) continue;

            // Collision layers - skip ignored ones!
            if (thisColl->ignores(other->getLayer())) continue;

            // No self-intersection test
            if (other->getId() == thisColl->getId()) continue;

            // Two static colliders can't collide
            if (other->getSibling<CTransform>()->is_static() &&
                    thisColl->getSibling<CTransform>()->is_static()) {
                continue;
            }

            if (thisColl->getCollisionVolume()->collide(other->getCollisionVolume()))
            {
                thisColl->onCollide(other->getParent());
                other->onCollide(thisColl->getParent());
            }
        }

        resolved.insert(thisColl);
    }
}

void CollisionSystem::draw()
{
    QSetIterator<std::shared_ptr<CCollider>> it(m_colliders);
    while (it.hasNext())
    {
        std::shared_ptr<CCollider> coll = it.next();
        if (coll->getLayer() == CCollider::DEFAULT) {
            coll->getCollisionVolume()->drawWireframe();
        }
    }
}
