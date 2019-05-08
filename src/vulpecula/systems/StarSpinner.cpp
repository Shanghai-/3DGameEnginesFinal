#include "StarSpinner.h"

#include "engine/objectManagement/GameWorld.h"
#include "engine/components/CTransform.h"

StarSpinner::StarSpinner(int priority) :
    System(priority)
{
}

StarSpinner::~StarSpinner()
{
}

QString StarSpinner::getComponentType() const
{
    return typeid(CStar).name();
}

inline uint qHash(const std::shared_ptr<CStar> &key) {
    return key->getId();
}

void StarSpinner::addComponent(const std::shared_ptr<Component> &c)
{
    auto s = std::dynamic_pointer_cast<CStar>(c);
    m_stars.insert(s);
}

void StarSpinner::removeComponent(const std::shared_ptr<Component> &c)
{
    auto s = std::dynamic_pointer_cast<CStar>(c);
    m_stars.remove(s);
}

void StarSpinner::tick(float seconds)
{
    QSetIterator<std::shared_ptr<CStar>> it(m_stars);
    while (it.hasNext()) {
        std::shared_ptr<CStar> s = it.next();

        float rise = glm::sin(m_gameWorld->getElapsedTime()) * 0.2f;

        s->getSibling<CTransform>()->pos = s->getOriginalPosition() + glm::vec3(0, rise + 0.5f, 0);
    }
}
