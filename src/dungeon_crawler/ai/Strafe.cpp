#include "Strafe.h"

#include "engine/util/CommonIncludes.h"
#include "engine/components/CTransform.h"
#include "engine/components/AIComponent.h"

Strafe::Strafe(AIComponent *owner) :
    m_owner(owner)
{
}

Status Strafe::act(float seconds)
{
    std::shared_ptr<CTransform> tf = m_owner->getSibling<CTransform>();
    std::shared_ptr<CTransform> player = m_owner->getBlackboard()->getComponent<CTransform>("player_transform");

    glm::vec3 between = tf->pos - player->pos;
    between.y = 0.0f;
    between = glm::normalize(between);
    glm::vec3 dir = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), between));
    tf->translate(dir * 2.8f * seconds);

    //tf->setRotation(dir);

    return SUCCESS;
}

void Strafe::reset()
{
    // No stored state -> nothing to reset
}
