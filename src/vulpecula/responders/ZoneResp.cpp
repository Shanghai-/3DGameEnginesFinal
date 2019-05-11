#include "ZoneResp.h"

#include "engine/components/CRenderable.h"

ZoneResp::ZoneResp(std::shared_ptr<RandomAudioSource> s) :
    m_src(s)
{
}

ZoneResp::~ZoneResp()
{
}

void ZoneResp::onCollide(std::shared_ptr<GameObject> other)
{
    if (other->getName() == "Player") {
        m_src->play();
    }
}

void ZoneResp::onCollisionEnd(std::shared_ptr<GameObject> other)
{
    if (other->getName() == "Player") {
        m_src->pause();
    }
}
