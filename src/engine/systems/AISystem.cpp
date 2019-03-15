#include "AISystem.h"

AISystem::AISystem(int capacity) :
    System(capacity)
{
}

AISystem::~AISystem()
{
}

QString AISystem::getComponentType() const
{
    return typeid(AIComponent).name();
}

inline uint qHash(const std::shared_ptr<AIComponent> &key) {
    return key->getId();
}

//void AISystem::addComponent(const std::shared_ptr<Component> &c)
//{
//    std::shared_ptr<AIComponent> comp = std::dynamic_pointer_cast<AIComponent>(c);
//    m_agents.insert(comp);
//}

void AISystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<AIComponent> comp = std::dynamic_pointer_cast<AIComponent>(c);
    m_agents.remove(comp);
}
