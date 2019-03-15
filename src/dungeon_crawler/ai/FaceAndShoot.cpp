#include "FaceAndShoot.h"

#include "engine/util/CommonIncludes.h"
#include "engine/components/CTransform.h"
#include "engine/components/AIComponent.h"
#include "engine/components/CRenderable.h"
#include "engine/components/CCollider.h"
#include "engine/components/volumes/CollCylinder.h"
#include "dungeon_crawler/CBullet.h"

#include <random>

FaceAndShoot::FaceAndShoot(std::shared_ptr<AIComponent> owner, std::shared_ptr<GameWorld> gw) :
    m_owner(owner),
    m_gw(gw)
{
    m_cooldown = m_owner->getSibling<CCooldown>();
}

Status FaceAndShoot::act(float seconds)
{
    if (m_cooldown->isReady()) {
        std::random_device r;
        std::mt19937 rng;
        rng.seed(r());

        std::uniform_int_distribution<int> rand(0, 5);
        if (rand(rng) == 0) {
            auto tf = m_owner->getSibling<CTransform>();
            auto player = m_owner->getBlackboard()->getComponent<CTransform>("player_transform");
            glm::vec3 dir = glm::normalize(player->pos + glm::vec3(0.0f, 0.5f, 0.0f) - tf->pos);

            std::shared_ptr<GameObject> bullet = std::make_shared<GameObject>("Bullet", m_gw->getNewObjID());
            bullet->addComponent(std::make_shared<CTransform>(bullet, false, tf->pos + glm::vec3(0.0f, 0.1f, 0.0f),
                                                              glm::vec3(0.0f), glm::vec3(0.2f)));
            bullet->addComponent(std::make_shared<CRenderable>(bullet, "sphere", "bullet"));
            bullet->addComponent(std::make_shared<CBullet>(bullet, tf, dir, 2.3f, 60.0f));
            auto coll = std::make_shared<CollCylinder>(glm::vec3(0.0f, -0.1f, 0.0f), 0.2f, 0.2f);
            bullet->addComponent(std::make_shared<CCollider>(bullet, coll, true));
            m_gw->addGameObject(bullet);

            m_cooldown->begin();
            return SUCCESS;
        }
        return RUNNING;
    }

    return FAIL;
}

void FaceAndShoot::reset()
{

}
