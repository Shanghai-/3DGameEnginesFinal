#ifndef COLELLIPSOID_H
#define COLELLIPSOID_H

#include "engine/components/Component.h"
#include <glm/vec3.hpp>

class ColEllipsoid : public Component
{
public:
    ColEllipsoid(std::shared_ptr<GameObject> parent, glm::vec3 radii);
    ~ColEllipsoid();

    glm::vec3 getRadii();
    void drawWireframe();

private:
    glm::vec3 m_radii;
};

#endif // COLELLIPSOID_H
