#include "ShootSystem.h"

#include "glm/gtx/norm.hpp"
#include "engine/components/CCamera.h"
#include "engine/objectManagement/GameWorld.h"
#include "engine/systems/RaycastSystem.h"
#include "CHealth.h"
#include "engine/components/CRenderable.h"
#include <iostream>

ShootSystem::ShootSystem(int priority, std::shared_ptr<GameObject> player,
                         std::shared_ptr<GameWorld> gw) :
    System(priority),
    m_gw(gw)
{
    m_input = InputManager::getGlobalInstance();
    m_playerLoc = player->getComponent<CTransform>();
    m_playerCooldown = player->getComponent<CCooldown>();
}

ShootSystem::~ShootSystem()
{
}

QString ShootSystem::getComponentType() const
{
    return typeid(CBullet).name();
}

inline uint qHash(const std::shared_ptr<CBullet> &key) {
    return key->getId();
}

void ShootSystem::addComponent(const std::shared_ptr<Component> &c)
{
    auto comp = std::dynamic_pointer_cast<CBullet>(c);
    m_bullets.insert(comp);
}

void ShootSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    auto comp = std::dynamic_pointer_cast<CBullet>(c);
    m_bullets.remove(comp);
}

void ShootSystem::tick(float seconds)
{
    QSetIterator<std::shared_ptr<CBullet>> it(m_bullets);
    while (it.hasNext()) {
        std::shared_ptr<CBullet> b = it.next();
        std::shared_ptr<CTransform> bt = b->getSibling<CTransform>();

        bt->translate(b->getDirection() * b->getSpeed() * seconds);

        if (glm::distance2(bt->pos, b->getOwnerTransform()->pos) > b->getMaxDist())
        {
            m_gw->removeGameObject(b->getParent());
        }
    }

    if (m_input->isPressed(Qt::LeftButton) && m_playerCooldown->isReady()) {
        std::shared_ptr<CCamera> playerCam = m_playerCooldown->getSibling<CCamera>();

        RaycastSystem::RaycastHit res = m_gw->getRaycaster()->cast(m_playerLoc->toWorld(playerCam->getLocalPos()),
                                                                   playerCam->getCameraObject()->getLook(),
                                                                   CCollider::DEFAULT);

        if (res.didHit && res.obj->getName() == "Enemy") {
            std::shared_ptr<CHealth> h = res.obj->getComponent<CHealth>();
            h->removeHealth(1);

            if (h->getCurrHealth() <= 0) {
                m_gw->removeGameObject(res.obj);
            }
        }

        m_playerCooldown->begin();
    }
}
