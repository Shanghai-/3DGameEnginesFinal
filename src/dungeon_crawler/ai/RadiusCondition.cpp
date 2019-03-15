#include "RadiusCondition.h"

#include "engine/components/CTransform.h"
#include "engine/components/AIComponent.h"
#include "glm/gtx/norm.hpp"

RadiusCondition::RadiusCondition(AIComponent *owner, float r) :
    m_owner(owner),
    m_radius(r)
{
}

Status RadiusCondition::query()
{
    std::shared_ptr<CTransform> player = m_owner->getBlackboard()->getComponent<CTransform>("player_transform");
    std::shared_ptr<CTransform> self = m_owner->getSibling<CTransform>();

    return (glm::distance2(player->pos, self->pos) < m_radius) ? SUCCESS : FAIL;
}

void RadiusCondition::reset()
{
    // Don't need to do anything special to reset because there's no internal state
}
