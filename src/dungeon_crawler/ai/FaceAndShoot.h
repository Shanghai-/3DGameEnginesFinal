#ifndef FACEANDSHOOT_H
#define FACEANDSHOOT_H

#include "engine/ai/Action.h"
#include "dungeon_crawler/CCooldown.h"
#include "engine/objectManagement/GameWorld.h"

class AIComponent;

class FaceAndShoot : public Action
{
public:
    FaceAndShoot(std::shared_ptr<AIComponent> owner, std::shared_ptr<GameWorld> gw);

    Status act(float seconds);
    void reset();

private:
    std::shared_ptr<AIComponent> m_owner;
    std::shared_ptr<GameWorld> m_gw;
    std::shared_ptr<CCooldown> m_cooldown;
};

#endif // FACEANDSHOOT_H
