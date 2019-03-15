#ifndef PLAYERCOLLISION_H
#define PLAYERCOLLISION_H

#include "engine/components/responders/CollisionResponse.h"
#include "engine/objectManagement/GameWorld.h"

class PlayerCollision : public CollisionResponse
{
public:
    PlayerCollision(std::shared_ptr<GameObject> player,
                    std::shared_ptr<GameWorld> gw);
    ~PlayerCollision();

    void onCollide(std::shared_ptr<GameObject> other);

private:
    std::shared_ptr<GameObject> m_player;
    std::shared_ptr<GameWorld> m_gw;
};

#endif // PLAYERCOLLISION_H
