#ifndef GUITARSTARRESP_H
#define GUITARSTARRESP_H

#include "engine/components/responders/CollisionResponse.h"
#include "engine/components/CAudioSource.h"

class GuitarStarResp : public CollisionResponse
{
public:
    GuitarStarResp(std::shared_ptr<GameObject> star, std::shared_ptr<GameObject> zone, GameWorld *gw);
    ~GuitarStarResp();

    void onCollide(std::shared_ptr<GameObject> other);
    void onCollisionEnd(std::shared_ptr<GameObject> other);

private:
    std::shared_ptr<GameObject> m_star, m_zone;
    GameWorld *m_gw;
};

#endif // GUITARSTARRESP_H
