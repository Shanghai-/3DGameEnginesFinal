#ifndef COLLSPHERE_H
#define COLLSPHERE_H

#include "CollisionVolume.h"

class CollSphere : public CollisionVolume
{
public:
    CollSphere(glm::vec3 localPos, float radius);
    ~CollSphere();

    bool collideBox(CollBox *other);
    bool collideSphere(CollSphere *other);
    bool collideCylinder(CollCylinder *other);
    bool collideComposite(CollComposite *other);
    bool collide(std::shared_ptr<CollisionVolume> other);

    std::pair<glm::vec3, glm::vec3> getExtrema();

    float intersect(glm::vec3 origin, glm::vec3 direction);

    void drawWireframe();

private:
    float m_radius, m_r2;
};

#endif // COLLSPHERE_H
