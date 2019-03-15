#include "BackAway.h"

#include "engine/util/CommonIncludes.h"
#include "engine/components/AIComponent.h"
#include "engine/components/CTransform.h"

BackAway::BackAway(AIComponent *owner) :
    m_owner(owner)
{
}

Status BackAway::act(float seconds)
{
    std::shared_ptr<CTransform> tf = m_owner->getSibling<CTransform>();
    std::shared_ptr<CTransform> player = m_owner->getBlackboard()->getComponent<CTransform>("player_transform");

    glm::vec3 dir = tf->pos - player->pos;
    dir.y = 0.0f;
    dir = glm::normalize(dir);
    tf->translate(dir * 2.0f * seconds);
    return SUCCESS;
}

void BackAway::reset()
{
    // No stored state -> nothing to reset
}
