#ifndef ENEMYAISYSTEM_H
#define ENEMYAISYSTEM_H

#include "engine/systems/AISystem.h"
#include "engine/ai/Selector.h"

class EnemyAISystem : public AISystem
{
public:
    EnemyAISystem(int priority, std::shared_ptr<GameWorld> gw);
    ~EnemyAISystem();

    void addComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    std::shared_ptr<GameWorld> m_gw;
};

#endif // ENEMYAISYSTEM_H
