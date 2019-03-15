#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "engine/systems/System.h"

#include "warmup1/CustomComponents/CPhysics.h"

class PhysicsSystem : public System
{
public:
    PhysicsSystem(int priority);
    ~PhysicsSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    QSet<std::shared_ptr<CPhysics>> m_components;
};

#endif // PHYSICSSYSTEM_H
