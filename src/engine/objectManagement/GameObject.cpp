#include "GameObject.h"

#include "engine/components/Component.h"

#include <iostream>

GameObject::GameObject(QString name, const uint id) :
    m_name(name),
    m_id(id)
{
}

GameObject::~GameObject()
{
    m_components.clear();
}

QList<std::shared_ptr<Component>> GameObject::getComponentList() const
{
    return m_components.values();
}

QString GameObject::getName() const
{
    return m_name;
}

uint GameObject::getID() const
{
    return m_id;
}

void GameObject::finishRemoval(Component *c)
{
    c->deleteSelf();
}
