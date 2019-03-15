#include "Blackboard.h"

Blackboard::Blackboard()
{
}

Blackboard::~Blackboard()
{
}

void Blackboard::addComponent(QString name, std::shared_ptr<Component> comp)
{
    m_store[name] = comp;
}

void Blackboard::removeComponent(QString name)
{
    m_store.remove(name);
}
