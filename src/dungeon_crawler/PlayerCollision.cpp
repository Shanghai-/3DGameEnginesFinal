#include "PlayerCollision.h"

#include "CHealth.h"

PlayerCollision::PlayerCollision(std::shared_ptr<GameObject> player,
                                 std::shared_ptr<GameWorld> gw) :
    m_player(player),
    m_gw(gw)
{
}

PlayerCollision::~PlayerCollision()
{
}

void PlayerCollision::onCollide(std::shared_ptr<GameObject> other)
{
    if (other->getName() == "Item")
    {
        auto h = m_player->getComponent<CHealth>();
        if (h->getCurrHealth() < h->getMaxHealth()) {
            m_gw->removeGameObject(other);
            h->addHealth(2, true);
        }
    }

    if (other->getName() == "Bullet")
    {
        auto h = m_player->getComponent<CHealth>();
        h->removeHealth(1);
        m_gw->removeGameObject(other);
    }
}
