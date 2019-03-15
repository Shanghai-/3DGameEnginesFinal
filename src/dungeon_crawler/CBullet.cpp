#include "CBullet.h"

CBullet::CBullet(std::shared_ptr<GameObject> parent, std::shared_ptr<CTransform> owner,
                 glm::vec3 direction, float speed, float maxDist) :
    Component(parent),
    m_owner(owner),
    m_direction(direction),
    m_speed(speed),
    m_maxDist(maxDist)
{
}

CBullet::~CBullet()
{
}

glm::vec3 CBullet::getDirection()
{
    return m_direction;
}

float CBullet::getSpeed()
{
    return m_speed;
}

float CBullet::getMaxDist()
{
    return m_maxDist;
}

std::shared_ptr<CTransform> CBullet::getOwnerTransform()
{
    return m_owner;
}
