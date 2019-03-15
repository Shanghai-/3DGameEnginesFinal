#include "MoveToPlayer.h"

#include "glm/gtx/norm.hpp"
#include "engine/components/CTransform.h"
#include "engine/components/CNavMesh.h"
#include "engine/ai/AStarSearch.h"
#include "engine/components/AIComponent.h"

MoveToPlayer::MoveToPlayer(AIComponent *owner) :
    m_owner(owner)
{
}

Status MoveToPlayer::act(float seconds)
{
    if (!m_path.empty()) {
        glm::vec3 nextPos = m_path[0]->getPosition();
        std::shared_ptr<CTransform> tf = m_owner->getSibling<CTransform>();

        if (glm::distance2(nextPos, tf->pos) < 0.1f) {
            m_path.pop_front();
            return (m_path.isEmpty()) ? SUCCESS : act(seconds);

        } else {
            glm::vec3 dir = glm::normalize(nextPos - tf->pos);
            tf->translate(dir * 2.6f * seconds);
            return RUNNING;
        }
    }

    std::shared_ptr<CTransform> tf = m_owner->getSibling<CTransform>();
    std::shared_ptr<CTransform> player = m_owner->getBlackboard()->getComponent<CTransform>("player_transform");

    AStarNode *closestToTarget, *closestToSelf;
    float targetDist = std::numeric_limits<float>::infinity();
    float selfDist = std::numeric_limits<float>::infinity();

    QSetIterator<AStarNode *> it(m_owner->getBlackboard()->getComponent<CNavMesh>("navmesh")->getNodes());
    while (it.hasNext()) {
        AStarNode *n = it.next();

        float td = glm::distance2(n->getPosition(), player->pos);
        if (td < targetDist) {
            closestToTarget = n;
            targetDist = td;
        }

        float sd = glm::distance2(n->getPosition(), tf->pos);
        if (sd < selfDist) {
            closestToSelf = n;
            selfDist = sd;
        }

        n->onPath = false;
    }

    m_path = AStarSearch::getPath(closestToSelf, closestToTarget);
    return RUNNING;
}

void MoveToPlayer::reset()
{
    m_path.empty();
}
