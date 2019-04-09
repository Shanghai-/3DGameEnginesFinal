#include "ThirdPersonCamSys.h"

#include "engine/components/CTransform.h"

#define HORIZ_SPEED 0.0015f
#define VERT_SPEED 0.0010f

ThirdPersonCamSys::ThirdPersonCamSys(int priority, float min_zoom,
                                     float max_zoom, float initial_zoom) :
    CameraSystem(priority),
    m_min_zoom(min_zoom),
    m_max_zoom(max_zoom),
    m_cur_zoom(initial_zoom)
{
    m_input = InputManager::getGlobalInstance();
}

ThirdPersonCamSys::~ThirdPersonCamSys()
{
}

void ThirdPersonCamSys::updateCamera(std::shared_ptr<CCamera> camComp,
                                     std::shared_ptr<Camera> graphicsCam)
{
    std::shared_ptr<CTransform> transform = camComp->getSibling<CTransform>();

    glm::vec2 delta = m_input->getMouseDelta();
    m_input->resetMouseDelta();

    // For yaw, we can just subtract deltaX as much as we want because it wraps perfectly
    graphicsCam->setYaw(graphicsCam->getYaw() - delta.x * HORIZ_SPEED);

    // For pitch, we need to clamp the value so that the player can't look so far down or
    // so far up that the camera circles around
    float newPitch = glm::clamp(graphicsCam->getPitch() - delta.y * VERT_SPEED, -1.4f, 1.5f);
    graphicsCam->setPitch(newPitch);

    float a = m_input->getScrollAngle();
    if (a > 14.0f) {
        m_input->resetScrollAngle();
        m_cur_zoom = m_cur_zoom * 0.75f;

    } else if (a < -14.0f) {
        m_input->resetScrollAngle();
        m_cur_zoom = m_cur_zoom * 1.25f;

    }

    m_cur_zoom = glm::clamp(m_cur_zoom, m_min_zoom, m_max_zoom);

    glm::vec3 pose = transform->pos + camComp->getLocalPos();
    pose -= graphicsCam->getLook() * m_cur_zoom;
    graphicsCam->setEye(pose);
}
