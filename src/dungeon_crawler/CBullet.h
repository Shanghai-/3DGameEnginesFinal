#ifndef CBULLET_H
#define CBULLET_H

#include "engine/components/Component.h"
#include "engine/components/CTransform.h"

class CBullet : public Component
{
public:
    CBullet(std::shared_ptr<GameObject> parent, std::shared_ptr<CTransform> owner,
            glm::vec3 direction, float speed, float maxDist);
    ~CBullet();

    glm::vec3 getDirection();
    float getSpeed();
    float getMaxDist();

    std::shared_ptr<CTransform> getOwnerTransform();

private:
    std::shared_ptr<CTransform> m_owner;

    glm::vec3 m_direction;
    float m_speed;
    float m_maxDist;
};

#endif // CBULLET_H
