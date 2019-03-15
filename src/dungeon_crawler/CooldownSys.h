#ifndef COOLDOWNSYS_H
#define COOLDOWNSYS_H

#include "engine/systems/System.h"
#include "CCooldown.h"

class CooldownSys : public System
{
public:
    CooldownSys(int priority);
    ~CooldownSys();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    QSet<std::shared_ptr<CCooldown>> m_components;
};

#endif // COOLDOWNSYS_H
