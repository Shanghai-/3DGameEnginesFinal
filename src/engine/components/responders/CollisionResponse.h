#ifndef COLLISIONRESPONSE_H
#define COLLISIONRESPONSE_H

#include "engine/objectManagement/GameObject.h"

class CollisionResponse
{
public:
    CollisionResponse() {}
    ~CollisionResponse() {}

    virtual void onCollide(std::shared_ptr<GameObject> other) = 0;
    virtual void onCollisionEnd(std::shared_ptr<GameObject> other) {}
};

#endif // COLLISIONRESPONSE_H
