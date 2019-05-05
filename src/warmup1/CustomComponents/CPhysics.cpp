#include "CPhysics.h"

CPhysics::CPhysics(std::shared_ptr<GameObject> parent) :
    Component(parent),
    vel(glm::vec3(0.0f)),
    acc(glm::vec3(0.0f)),
    canJump(true),
    m_gravity(glm::vec3(0.0f, -9.1f, 0.0f))
{
}

CPhysics::CPhysics(std::shared_ptr<GameObject> parent, glm::vec3 grav) :
    Component(parent),
    vel(glm::vec3(0.f)),
    acc(grav),
    canJump(true),
    m_gravity(grav)
{
    vel = grav;
}

CPhysics::~CPhysics()
{
}

glm::vec3 CPhysics::getGravity()
{
    return m_gravity;
}
