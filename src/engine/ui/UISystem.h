#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "UIComponent.h"
#include <QString>

class UISystem
{
public:
    UISystem(int priority) : m_priority(priority) {}
    ~UISystem() {}

    virtual QString getComponentType() const = 0;

    virtual void addComponent(const std::shared_ptr<UIComponent> &c) = 0;
    virtual void removeComponent(const std::shared_ptr<UIComponent> &c) = 0;

    virtual void tick(float seconds) {}
    virtual void draw() {}
    virtual void resize(int w, int h) {}

    int getPriority() const { return m_priority; }

private:
    int m_priority;
};

#endif // UISYSTEM_H
