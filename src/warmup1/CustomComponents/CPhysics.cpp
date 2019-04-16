#include "CPhysics.h"

CPhysics::CPhysics(std::shared_ptr<GameObject> parent) :
    Component(parent),
    vel(glm::vec3(0.0f)),
    acc(glm::vec3(0.0f)),
    canJump(true)
{
}

CPhysics::CPhysics(std::shared_ptr<GameObject> parent, glm::vec3 grav) :
    Component(parent),
    vel(glm::vec3(0.f)),
    acc(grav),
    canJump(true)
{

}

CPhysics::~CPhysics()
{
}
