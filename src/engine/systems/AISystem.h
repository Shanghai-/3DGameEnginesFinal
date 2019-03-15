#ifndef AISYSTEM_H
#define AISYSTEM_H

#include "System.h"
#include "engine/components/AIComponent.h"

class AISystem : public System
{
public:
    AISystem(int priority);
    ~AISystem();

    QString getComponentType() const override;
    //void addComponent(const std::shared_ptr<Component> &c) override;
    void removeComponent(const std::shared_ptr<Component> &c) override;

protected:
    QSet<std::shared_ptr<AIComponent>> m_agents;
};

#endif // AISYSTEM_H
