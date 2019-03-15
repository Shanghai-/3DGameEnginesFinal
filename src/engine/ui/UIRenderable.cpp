#include "UIRenderable.h"


UIRenderable::UIRenderable(std::shared_ptr<UIObject> parent, UITransform t, QString shape) :
    UIRenderable(parent, t, shape, "default")
{
}

UIRenderable::UIRenderable(std::shared_ptr<UIObject> parent, UITransform t, QString shape, QString material) :
    UIComponent(parent),
    m_transform(t),
    m_shape(shape),
    m_material(material)
{
}

UIRenderable::~UIRenderable()
{
}

UITransform *UIRenderable::getTransform() { return &m_transform; }
std::string UIRenderable::getShapeName() { return m_shape.toStdString(); }
std::string UIRenderable::getMaterial() { return m_material.toStdString(); }
void UIRenderable::setMaterial(QString m) { m_material = m; }

glm::vec2 UIRenderable::getDimensions()
{
    return m_transform.getScale(); // TODO: adapt this to mesh size
}

void UIRenderable::receipts()
{
    // nada
}
