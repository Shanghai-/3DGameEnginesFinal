#include "CollisionSystem.h"

#include "engine/graphics/Graphics.h"

#include <iostream>
#include <utility>

CollisionSystem::CollisionSystem(int priority) :
    System(priority)
{
    m_type = QString::fromStdString(typeid(CCollider).name());

    Material debug;
    debug.useLighting = false;
    debug.color = glm::vec3(1.0f, 0.666f, 0.08f);

    m_graphics = Graphics::getGlobalInstance();
    m_graphics->addMaterial("Debug_Collision", debug);
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
}

void CollisionSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CCollider> coll = std::dynamic_pointer_cast<CCollider>(c);
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

            // TODO/NOTE: Originally we only tracked start/continue/end for triggers
            // and not solid volumes. However, a situation came up where it was useful
            // for solid objects, so I changed it.
            //bool isATrigger = thisColl->isTrigger() || other->isTrigger();
            bool isATrigger = true;

            bool alreadyColliding = isATrigger &&
                    (m_currently_colliding.contains(thisColl, other)
                    || m_currently_colliding.contains(other, thisColl));

            if (thisColl->getCollisionVolume()->collide(other->getCollisionVolume()))
            {
                if (isATrigger) {
                    if (alreadyColliding) {
                        thisColl->onCollisionContinue(other->getParent());
                        other->onCollisionContinue(thisColl->getParent());

                    } else {
                        m_currently_colliding.insert(thisColl, other);
                        thisColl->onCollide(other->getParent());
                        other->onCollide(thisColl->getParent());
                    }

                } else {
                    // TODO/NOTE: This clause should never hit currently
                    // (see NOTE above)
                    thisColl->onCollide(other->getParent());
                    other->onCollide(thisColl->getParent());
                }

            } else if (alreadyColliding) {

                m_currently_colliding.remove(thisColl, other);
                m_currently_colliding.remove(other, thisColl);

                thisColl->onCollisionEnd(other->getParent());
                other->onCollisionEnd(thisColl->getParent());
            }
        }

        resolved.insert(thisColl);
    }
}

void CollisionSystem::draw()
{

    QSetIterator<std::shared_ptr<CCollider>> it(m_colliders);
    m_graphics->setMaterial("Debug_Collision");
    while (it.hasNext())
    {
        std::shared_ptr<CCollider> coll = it.next();
        /* if (coll->getLayer() == CCollider::DEFAULT) {
            coll->getCollisionVolume()->drawWireframe();
        } */
        coll->getCollisionVolume()->drawWireframe();
    }
}
