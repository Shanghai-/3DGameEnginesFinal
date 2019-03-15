#include "FPSCameraSystem.h"

#include "engine/components/CTransform.h"
#include "engine/objectManagement/GameWorld.h"

#define LOOK_SPEED 0.0015f

FPSCameraSystem::FPSCameraSystem(int priority) :
    CameraSystem(priority)
{
    m_input = InputManager::getGlobalInstance();
}

FPSCameraSystem::~FPSCameraSystem()
{
}

void FPSCameraSystem::updateCamera(std::shared_ptr<CCamera> camComp,
                                   std::shared_ptr<Camera> graphicsCam)
{
    std::shared_ptr<CTransform> transform = camComp->getSibling<CTransform>();

    glm::vec2 delta = m_input->getMouseDelta();
    m_input->resetMouseDelta();

    // For yaw, we can just subtract deltaX as much as we want because it wraps perfectly
    graphicsCam->setYaw(graphicsCam->getYaw() - delta.x * LOOK_SPEED);

    // For pitch, we need to clamp the value so that the player can't look so far down or
    // so far up that the camera circles around
    float newPitch = glm::clamp(graphicsCam->getPitch() - delta.y * LOOK_SPEED, -1.4f, 1.5f);
    graphicsCam->setPitch(newPitch);

    glm::vec3 pose = transform->pos + camComp->getLocalPos();
    graphicsCam->setEye(pose);
}
