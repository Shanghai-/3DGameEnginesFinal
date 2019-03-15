#ifndef SHOOTSYSTEM_H
#define SHOOTSYSTEM_H

#include "engine/systems/System.h"
#include "engine/input/InputManager.h"
#include "engine/components/CTransform.h"
#include "engine/objectManagement/GameObject.h"
#include "engine/objectManagement/GameWorld.h"
#include "CBullet.h"
#include "CCooldown.h"

class ShootSystem : public System
{
public:
    ShootSystem(int priority, std::shared_ptr<GameObject> player, std::shared_ptr<GameWorld> gw);
    ~ShootSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    std::shared_ptr<GameWorld> m_gw;
    InputManager *m_input;
    std::shared_ptr<CTransform> m_playerLoc;
    std::shared_ptr<CCooldown> m_playerCooldown;

    QSet<std::shared_ptr<CBullet>> m_bullets;
};

#endif // SHOOTSYSTEM_H
