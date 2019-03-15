#ifndef COLLCYLINDER_H
#define COLLCYLINDER_H

#include "CollisionVolume.h"
#include "CollBox.h"
#include "CollSphere.h"
#include "CollComposite.h"

class CollCylinder : public CollisionVolume
{
public:
    CollCylinder(glm::vec3 localPos, float height, float radius);
    ~CollCylinder();

    bool collideBox(CollBox *other);
    bool collideSphere(CollSphere *other);
    bool collideCylinder(CollCylinder *other);
    bool collideComposite(CollComposite *other);
    bool collide(std::shared_ptr<CollisionVolume> other);

    std::pair<glm::vec3, glm::vec3> getExtrema();

    float intersect(glm::vec3 origin, glm::vec3 direction);

    void drawWireframe();

    float getRadius() const { return m_radius; }
    float getHeight() const { return m_height; }

private:
    float m_height, m_radius, m_r2;
};

#endif // COLLCYLINDER_H
