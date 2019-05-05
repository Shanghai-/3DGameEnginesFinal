#ifndef CPHYSICS_H
#define CPHYSICS_H

#include "engine/util/CommonIncludes.h"
#include "engine/components/Component.h"
#include "engine/input/InputManager.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/Camera.h"

class CPhysics : public Component
{
public:
    CPhysics(std::shared_ptr<GameObject> parent);
    CPhysics(std::shared_ptr<GameObject> parent, glm::vec3 grav);
    ~CPhysics();

    glm::vec3 getGravity();

    glm::vec3 vel, acc;
    bool canJump;

private:
    glm::vec3 m_gravity;
};

#endif // CPHYSICS_H
