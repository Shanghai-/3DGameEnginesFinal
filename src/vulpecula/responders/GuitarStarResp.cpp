#include "GuitarStarResp.h"

#include "engine/components/CCollider.h"
#include "engine/components/CRenderable.h"
#include "engine/components/CAudioSource.h"
#include "engine/objectManagement/GameWorld.h"
#include "engine/graphics/Graphics.h"
#include "vulpecula/components/CStar.h"

GuitarStarResp::GuitarStarResp(std::shared_ptr<GameObject> star,
                               std::shared_ptr<GameObject> zone,
                               GameWorld *gw) :
    m_star(star),
    m_zone(zone),
    m_gw(gw)
{
}

GuitarStarResp::~GuitarStarResp()
{
}

void GuitarStarResp::onCollide(std::shared_ptr<GameObject> other)
{
    if (other->getName() == "Player") {
        auto audio = m_star->getComponent<CAudioSource>();
        audio->setMuted(false);

        m_gw->removeGameObject(m_zone);

        m_star->removeComponent<CRenderable>();
        m_star->removeComponent<CCollider>();
        m_star->removeComponent<CStar>();
    }
}

void GuitarStarResp::onCollisionEnd(std::shared_ptr<GameObject> other)
{
}
