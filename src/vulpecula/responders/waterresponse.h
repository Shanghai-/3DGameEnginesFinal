#ifndef WATERRESPONSE_H
#define WATERRESPONSE_H

#include "engine/components/responders/CollisionResponse.h"
#include "engine/components/CAudioSource.h"

class WaterResponse : public CollisionResponse
{
public:
    WaterResponse();
    ~WaterResponse();

    void onCollide(std::shared_ptr<GameObject> other);
};

#endif // WATERRESPONSE_H
