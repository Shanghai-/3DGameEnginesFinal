#ifndef COLLISIONVOLUME_H
#define COLLISIONVOLUME_H

#include <glm/glm.hpp>
#include <memory>
#include <utility>

class CollBox;
class CollSphere;
class CollCylinder;
class CollComposite;
class CCollider;

class CollisionVolume
{
public:
    CollisionVolume(glm::vec3 localPos) : m_localPos(localPos) {}
    ~CollisionVolume() {}

    virtual bool collideBox(CollBox *other) = 0;
    virtual bool collideSphere(CollSphere *other) = 0;
    virtual bool collideCylinder(CollCylinder *other) = 0;
    virtual bool collideComposite(CollComposite *other) = 0;
    virtual bool collide(std::shared_ptr<CollisionVolume> other) = 0;

    virtual std::pair<glm::vec3, glm::vec3> getExtrema() = 0;

    virtual float intersect(glm::vec3 origin, glm::vec3 direction) = 0;
    //virtual RaycastSystem::RaycastHit getHitInfo(float t) = 0;

    virtual void drawWireframe() = 0;

    const glm::vec3 getLocalPos() {
        return m_localPos;
    }

    CCollider *getOwner()
    {
        return m_owner;
    }

    virtual void setOwner(CCollider *o)
    {
        m_owner = o;
    }

protected:
    const glm::vec3 m_localPos;
    CCollider *m_owner;
};

#endif // COLLISIONVOLUME_H
