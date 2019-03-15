#include "InputSystem.h"

#include "engine/input/InputManager.h"
#include "engine/graphics/Graphics.h"
#include "warmup1/CustomComponents/CPhysics.h"
#include "engine/components/CCamera.h"

InputSystem::InputSystem(int priority) :
    System(priority)
{
    m_input = InputManager::getGlobalInstance();
    m_graphics = Graphics::getGlobalInstance();
}

InputSystem::~InputSystem()
{
}

QString InputSystem::getComponentType() const
{
    return typeid(CInputReceiver).name();
}

inline uint qHash(const std::shared_ptr<CInputReceiver> &key) {
    return key->getId();
}

void InputSystem::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CInputReceiver> r = std::dynamic_pointer_cast<CInputReceiver>(c);
    m_components.insert(r);
}

void InputSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CInputReceiver> r = std::dynamic_pointer_cast<CInputReceiver>(c);
    m_components.remove(r);
}

void InputSystem::tick(float seconds)
{
    std::shared_ptr<CCamera> cam = m_graphics->getActiveCamera();
    std::shared_ptr<Camera> graphicsCam = cam->getCameraObject();

    glm::vec3 forward = graphicsCam->getLook();
    forward.y = 0;
    forward = glm::normalize(forward) * 6.0f;

    glm::vec3 left = glm::cross(graphicsCam->getUp(), graphicsCam->getLook());
    left.y = 0;
    left = glm::normalize(left) * 6.0f;

    // Use separate acceleration accumulators for forward and sideways movement
    glm::vec3 accel_f = glm::vec3(0.0f);
    glm::vec3 accel_s = glm::vec3(0.0f);

    if (m_input->isPressed(Qt::Key_W)) {
        accel_f += forward;
    }
    if (m_input->isPressed(Qt::Key_A)) {
        accel_s += left;
    }
    if (m_input->isPressed(Qt::Key_S)) {
        accel_f -= forward;
    }
    if (m_input->isPressed(Qt::Key_D)) {
        accel_s -= left;
    }

    glm::vec3 accel_sum = accel_f + accel_s;

    // If the player is walking forward AND strafing, we average their
    // two motion vectors to make a diagonal walk that isn't 2x as fast
    if (glm::length2(accel_f) > 0.0f && glm::length2(accel_s) > 0.0f) {
        accel_sum = accel_sum / 2.0f;
    }

    QSetIterator<std::shared_ptr<CInputReceiver>> it(m_components);
    while (it.hasNext()) {
        std::shared_ptr<CPhysics> phys = it.next()->getSibling<CPhysics>();

        if (m_input->isPressed(Qt::Key_Space)) {
            if (phys->canJump) {
                phys->acc.y = 82.0f;
                phys->canJump = false;
            }
        }

        // We don't disturb the y acceleration, but we replace the x and z
        // acceleration with our newly calculated one
        phys->acc = (phys->acc * glm::vec3(0.0f, 1.0f, 0.0f)) + accel_sum;
    }
}
