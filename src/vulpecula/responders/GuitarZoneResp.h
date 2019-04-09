#ifndef COLLGUITAR_H
#define COLLGUITAR_H

#include "engine/components/responders/CollisionResponse.h"
#include "vulpecula/components/RandomAudioSource.h"

class GuitarZoneResp : public CollisionResponse
{
public:
    GuitarZoneResp(std::shared_ptr<RandomAudioSource> s);
    ~GuitarZoneResp();

    void onCollide(std::shared_ptr<GameObject> other);
    void onCollisionEnd(std::shared_ptr<GameObject> other);

private:
    std::shared_ptr<RandomAudioSource> m_src;
};

#endif // COLLGUITAR_H
