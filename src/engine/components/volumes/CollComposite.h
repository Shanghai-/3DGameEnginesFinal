#ifndef COLLCOMPOSITE_H
#define COLLCOMPOSITE_H

#include "CollisionVolume.h"

#include <QVector>

class CollComposite : public CollisionVolume
{
public:
    CollComposite();
    ~CollComposite();

    bool collideBox(CollBox *other);
    bool collideSphere(CollSphere *other);
    bool collideCylinder(CollCylinder *other);
    bool collideComposite(CollComposite *other);
    bool collide(std::shared_ptr<CollisionVolume> other);

    std::pair<glm::vec3, glm::vec3> getExtrema();

    float intersect(glm::vec3 origin, glm::vec3 direction);

    void setOwner(CCollider *o) override;

    void drawWireframe();

    void addCollider(const std::shared_ptr<CollisionVolume> &c);
private:
    QVector<std::shared_ptr<CollisionVolume>> m_volumes;

    float min_x, min_y, min_z, max_x, max_y, max_z;
};

#endif // COLLCOMPOSITE_H
