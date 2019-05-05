#ifndef STANDABLE_H
#define STANDABLE_H

#include "engine/components/responders/CollisionResponse.h"
#include "engine/components/CTransform.h"

class Standable : public CollisionResponse
{
public:
    Standable(CTransform *transform, glm::vec3 localPos);
    ~Standable();

    void onCollide(std::shared_ptr<GameObject> other);
    void onCollisionEnd(std::shared_ptr<GameObject> other);
    void onCollisionContinue(std::shared_ptr<GameObject> other);

private:
    CTransform *m_transform;
    glm::vec3 m_localPos;
};

#endif // STANDABLE_H
