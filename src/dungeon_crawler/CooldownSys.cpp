#include "CooldownSys.h"

CooldownSys::CooldownSys(int priority) :
    System(priority)
{
}

CooldownSys::~CooldownSys()
{
}

QString CooldownSys::getComponentType() const
{
    return typeid(CCooldown).name();
}

inline uint qHash(const std::shared_ptr<CCooldown> &key) {
    return key->getId();
}

void CooldownSys::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CCooldown> comp = std::dynamic_pointer_cast<CCooldown>(c);
    m_components.insert(comp);
}

void CooldownSys::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CCooldown> comp = std::dynamic_pointer_cast<CCooldown>(c);
    m_components.remove(comp);
}

void CooldownSys::tick(float seconds)
{
    QSetIterator<std::shared_ptr<CCooldown>> it(m_components);
    while (it.hasNext()) {
        it.next()->decrease(seconds);
    }
}
