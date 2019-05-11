#ifndef STARRESP_H
#define STARRESP_H

#include "engine/components/responders/CollisionResponse.h"
#include "engine/components/CAudioSource.h"

class StarResp : public CollisionResponse
{
public:
    StarResp(std::shared_ptr<GameObject> star,
                   std::shared_ptr<GameObject> zone,
                   GameWorld *gw);
    ~StarResp();

    void onCollide(std::shared_ptr<GameObject> other);
    void onCollisionEnd(std::shared_ptr<GameObject> other);

private:
    std::shared_ptr<GameObject> m_star, m_zone;
    GameWorld *m_gw;
};

#endif // STARRESP_H
