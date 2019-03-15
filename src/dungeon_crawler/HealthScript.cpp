#include "HealthScript.h"

HealthScript::HealthScript(std::shared_ptr<CHealth> toCount) :
    m_healthComponent(toCount)
{
    m_hearts.reserve(toCount->getMaxHealth() + 1);
}

HealthScript::~HealthScript()
{
}

void HealthScript::addHeart(std::shared_ptr<UIRenderable> h)
{
    m_hearts.append(h);
}

void HealthScript::onTick(float seconds)
{
    for (int i = 0; i < m_hearts.size(); i++) {

        if (i < m_healthComponent->getCurrHealth()) {
            if (i < m_healthComponent->getMaxHealth()) {
                m_hearts[i]->setMaterial("heart_r");
            } else {
                m_hearts[i]->setMaterial("heart_g");
            }
        } else {
            if (i < m_healthComponent->getMaxHealth()) {
                m_hearts[i]->setMaterial("heart_e");
            } else {
                m_hearts[i]->setMaterial("hidden");
            }
        }
    }
}

void HealthScript::onDraw()
{
}

void HealthScript::onResize(int w, int h)
{
}
