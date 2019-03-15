#include "CHealth.h"

CHealth::CHealth(std::shared_ptr<GameObject> parent, int max) :
    Component(parent),
    m_maxHealth(max),
    m_currHealth(max)
{
}

CHealth::~CHealth()
{
}

void CHealth::addHealth(int to_add, bool overheal)
{
    m_currHealth += to_add;

    if (!overheal) m_currHealth = std::min(m_currHealth, m_maxHealth);
}

void CHealth::removeHealth(int to_remove)
{
    m_currHealth -= to_remove;
}

int CHealth::getMaxHealth()
{
    return m_maxHealth;
}

int CHealth::getCurrHealth()
{
    return m_currHealth;
}
