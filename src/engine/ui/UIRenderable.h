#ifndef UIRENDERABLE_H
#define UIRENDERABLE_H

#include "UIComponent.h"
#include "UITransform.h"

class UIRenderable : public UIComponent
{
public:
    UIRenderable(std::shared_ptr<UIObject> parent, UITransform t, QString shape);
    UIRenderable(std::shared_ptr<UIObject> parent,
                 UITransform t, QString shape, QString material);
    ~UIRenderable();

    UITransform *getTransform();
    std::string getShapeName();
    std::string getMaterial();
    void setMaterial(QString m);
    glm::vec2 getDimensions();

private:
    UITransform m_transform;
    QString m_shape;
    QString m_material;

    // UIComponent interface
public:
    void receipts();
};

#endif // UIRENDERABLE_H
