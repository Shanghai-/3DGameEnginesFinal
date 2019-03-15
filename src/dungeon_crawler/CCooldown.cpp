#include "CCooldown.h"

CCooldown::CCooldown(std::shared_ptr<GameObject> parent, float seconds) :
    Component(parent),
    m_cooldownLength(seconds),
    m_currSeconds(-1.0f)
{
}

CCooldown::~CCooldown()
{
}

void CCooldown::decrease(float seconds)
{
    if (m_currSeconds > 0.0f) m_currSeconds -= seconds;
}

void CCooldown::begin()
{
    m_currSeconds = m_cooldownLength;
}

bool CCooldown::isReady()
{
    return m_currSeconds <= 0.0f;
}
