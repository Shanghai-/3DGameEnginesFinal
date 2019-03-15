#ifndef CCamera_H
#define CCamera_H

#include "Component.h"
#include "engine/graphics/Camera.h"

class CCamera : public Component
{
public:
    CCamera(std::shared_ptr<GameObject> parent);
    CCamera(std::shared_ptr<GameObject> parent, glm::vec3 localPos);
    ~CCamera();

    bool isActive();
    std::shared_ptr<Camera> getCameraObject();

    void setLocalPos(glm::vec3 val);
    glm::vec3 getLocalPos();

    glm::vec4 getNegXPlane();
    glm::vec4 getNegYPlane();
    glm::vec4 getNegZPlane();
    glm::vec4 getPosXPlane();
    glm::vec4 getPosYPlane();
    glm::vec4 getPosZPlane();

    void setR0(glm::vec4 val);
    void setR1(glm::vec4 val);
    void setR2(glm::vec4 val);
    void setR3(glm::vec4 val);

private:
    bool m_isActive;
    std::shared_ptr<Camera> m_cam;
    glm::vec4 m_r0, m_r1, m_r2, m_r3;
    glm::vec3 m_localPos;
};

#endif // CCamera_H
