#ifndef WATERRESPONSE_H
#define WATERRESPONSE_H

#include "engine/components/responders/CollisionResponse.h"
#include "engine/components/CAudioSource.h"
#include "vulpecula/systems/PlayerMovementSys.h"

class WaterResponse : public CollisionResponse
{
public:
    WaterResponse(std::shared_ptr<PlayerMovementSys> mps);
    ~WaterResponse();

    void onCollide(std::shared_ptr<GameObject> other);
protected:
    std::shared_ptr<PlayerMovementSys> m_mps;
};

#endif // WATERRESPONSE_H
