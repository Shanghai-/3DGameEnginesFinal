#include "UIObject.h"

#include "UIComponent.h"

UIObject::UIObject(QString name, const uint id, Anchor anchor_pos) :
    m_name(name),
    m_id(id),
    m_pos(anchor_pos)
{
}

UIObject::~UIObject()
{
}

void UIObject::addComponent(std::shared_ptr<UIComponent> c) {
    m_components.append(c);
}

QVector<std::shared_ptr<UIComponent>> UIObject::getComponents() {
    return m_components;
}

Anchor UIObject::getAnchor()
{
    return m_pos;
}

QString UIObject::getName()
{
    return m_name;
}
