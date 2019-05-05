#include "Standable.h"

#include "warmup1/CustomComponents/CPhysics.h"

#include <iostream>

Standable::Standable(CTransform *transform, glm::vec3 localPos) :
    m_transform(transform),
    m_localPos(localPos)
{
}

Standable::~Standable()
{
}

void Standable::onCollide(std::shared_ptr<GameObject> other)
{
    float ypos = m_transform->pos.y + m_localPos.y;

    if (other->getComponent<CTransform>()->pos.y > ypos) {
        auto phys = other->getComponent<CPhysics>();
        if (phys) {
            phys->canJump = true;
            phys->acc = glm::vec3(0.0f, -0.01f, 0.0f);
            phys->vel = glm::vec3(0.0f, -0.01f, 0.0f);
        }
    }
}

void Standable::onCollisionEnd(std::shared_ptr<GameObject> other)
{
    auto phys = other->getComponent<CPhysics>();
    if (phys) {
        phys->acc = phys->getGravity();
    }
}

void Standable::onCollisionContinue(std::shared_ptr<GameObject> other)
{
    float ypos = m_transform->pos.y + m_localPos.y;

    if (other->getComponent<CTransform>()->pos.y > ypos) {
        auto phys = other->getComponent<CPhysics>();
        if (phys) {
            phys->canJump = true;
            phys->acc = glm::vec3(0.0f, -0.01f, 0.0f);
            phys->vel = glm::vec3(0.0f, -0.01f, 0.0f);
        }
    }
}
