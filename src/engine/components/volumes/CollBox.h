#ifndef COLLBOX_H
#define COLLBOX_H

#include "CollisionVolume.h"

class CollBox : public CollisionVolume
{
public:
    CollBox(glm::vec3 localPos, glm::vec3 dimensions);
    ~CollBox();

    bool collideBox(CollBox *other);
    bool collideSphere(CollSphere *other);
    bool collideCylinder(CollCylinder *other);
    bool collideComposite(CollComposite *other);
    bool collide(std::shared_ptr<CollisionVolume> other);

    std::pair<glm::vec3, glm::vec3> getExtrema();

    float intersect(glm::vec3 origin, glm::vec3 direction);

    void drawWireframe();

    glm::vec3 getDimensions();

private:
    glm::vec3 m_dims;
};

#endif // COLLBOX_H
