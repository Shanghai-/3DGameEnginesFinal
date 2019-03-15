#include "RaycastSystem.h"

#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "engine/components/CRenderable.h"

RaycastSystem::RaycastSystem() :
    System(1013) // arbitrary number cause it doesn't really matter
{
}

RaycastSystem::~RaycastSystem()
{
    m_colliders.empty();
}

QString RaycastSystem::getComponentType() const
{
    return typeid(CCollider).name();
}

inline uint qHash(const std::shared_ptr<CCollider> &key) {
    return key->getId();
}

void RaycastSystem::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CCollider> coll = std::dynamic_pointer_cast<CCollider>(c);
    m_colliders.insert(coll);
}

void RaycastSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CCollider> coll = std::dynamic_pointer_cast<CCollider>(c);
    m_colliders.remove(coll);
}

RaycastSystem::RaycastHit RaycastSystem::cast(glm::vec3 origin, glm::vec3 direction)
{
    return cast(origin, direction, -1);
}

RaycastSystem::RaycastHit RaycastSystem::cast(glm::vec3 origin, glm::vec3 direction, int layer)
{
    //glm::vec3 inverse_direction = 1.0f / direction;

    float min_t = std::numeric_limits<float>::infinity();
    std::shared_ptr<CCollider> best;

    QSetIterator<std::shared_ptr<CCollider>> it(m_colliders);
    while (it.hasNext())
    {
        std::shared_ptr<CCollider> coll = it.next();

        if (layer == -1 || coll->getLayer() == layer) {
            float this_t = coll->intersect(origin, direction);

            if (this_t > 0.0f && this_t < min_t) {
                min_t = this_t;
                best = coll;
            }
        }
    }

    if (min_t < std::numeric_limits<float>::infinity() && min_t >= 0.0f) {
        // TODO: implement normals
        return RaycastHit(true, best->getParent(), glm::vec3(0.0f));
    }

    return RaycastHit(false, nullptr, glm::vec3(0.0f));
}
