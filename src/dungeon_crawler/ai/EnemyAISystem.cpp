#include "EnemyAISystem.h"

#include "engine/ai/Sequence.h"
#include "RadiusCondition.h"
#include "MoveToPlayer.h"
#include "BackAway.h"
#include "Strafe.h"
#include "FaceAndShoot.h"

EnemyAISystem::EnemyAISystem(int priority, std::shared_ptr<GameWorld> gw) :
    AISystem(priority),
    m_gw(gw)
{
}

EnemyAISystem::~EnemyAISystem()
{
}

inline uint qHash(const std::shared_ptr<AIComponent> &key) {
    return key->getId();
}

void EnemyAISystem::tick(float seconds)
{
    QSetIterator<std::shared_ptr<AIComponent>> it(m_agents);
    while (it.hasNext())
    {
        it.next()->updateAITree(seconds);
    }
}

void EnemyAISystem::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<AIComponent> comp = std::dynamic_pointer_cast<AIComponent>(c);

    // Aggress sequence: if within aggro range, move to player
    Sequence *aggro = new Sequence();
    aggro->addChild(new RadiusCondition(comp.get(), 30.0f));
    aggro->addChild(new MoveToPlayer(comp.get()));

    // Avoidance sequence: if too close to player, move back
    Sequence *back_up = new Sequence();
    back_up->addChild(new RadiusCondition(comp.get(), 5.0f));
    back_up->addChild(new BackAway(comp.get()));

    // Shoot selector: If we aren't on cooldown, shoot; otherwise,
    // strafe around the player
    Selector *strafe_and_shoot = new Selector();
    strafe_and_shoot->addChild(new FaceAndShoot(comp, m_gw));
    strafe_and_shoot->addChild(new Strafe(comp.get()));

    // Attack sequence: If we're within attack range, shoot and strafe
    Sequence *attack = new Sequence();
    attack->addChild(new RadiusCondition(comp.get(), 15.0f));
    attack->addChild(strafe_and_shoot);

    // Root selector:
    // If too close, back up
    // If close enough, attack
    // If not super close but kinda close, get closer
    // If totally far away, nothing
    Selector *sel = new Selector();
    sel->addChild(back_up);
    sel->addChild(attack);
    sel->addChild(aggro);

    comp->setRoot(sel);

    m_agents.insert(comp);
}
