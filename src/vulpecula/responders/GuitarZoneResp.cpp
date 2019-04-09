#include "GuitarZoneResp.h"

#include "engine/components/CRenderable.h"

GuitarZoneResp::GuitarZoneResp(std::shared_ptr<RandomAudioSource> s) :
    m_src(s)
{
}

GuitarZoneResp::~GuitarZoneResp()
{
}

void GuitarZoneResp::onCollide(std::shared_ptr<GameObject> other)
{
    if (other->getName() == "Player") {
        m_src->play();
    }
}

void GuitarZoneResp::onCollisionEnd(std::shared_ptr<GameObject> other)
{
    if (other->getName() == "Player") {
        m_src->pause();
    }
}
