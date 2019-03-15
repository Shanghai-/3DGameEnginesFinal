#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include "UIObject.h"
#include "glm/glm.hpp"

class UIComponent
{
public:
    UIComponent(std::shared_ptr<UIObject> parent) : m_parent(parent) {}
    ~UIComponent() {}

    std::shared_ptr<UIObject> getParent() { return m_parent; }

    virtual void receipts() = 0;

protected:
    std::shared_ptr<UIObject> m_parent;
};

#endif // UICOMPONENT_H
