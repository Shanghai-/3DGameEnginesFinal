#ifndef ZONERESP_H
#define ZONERESP_H

#include "engine/components/responders/CollisionResponse.h"
#include "vulpecula/components/RandomAudioSource.h"

class ZoneResp : public CollisionResponse
{
public:
    ZoneResp(std::shared_ptr<RandomAudioSource> s);
    ~ZoneResp();

    void onCollide(std::shared_ptr<GameObject> other);
    void onCollisionEnd(std::shared_ptr<GameObject> other);

private:
    std::shared_ptr<RandomAudioSource> m_src;
};

#endif // ZONERESP_H
