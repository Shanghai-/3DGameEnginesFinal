#include "MovementSys.h"

#include "engine/components/CInputReceiver.h"
#include "engine/components/CCamera.h"
#include "engine/objectManagement/GameWorld.h"
#include "engine/systems/RaycastSystem.h"

#define MOVEMENT_SPEED 2.7f

MovementSys::MovementSys(int priority) :
    System(priority)
{
    m_input = InputManager::getGlobalInstance();
    m_graphics = Graphics::getGlobalInstance();
}

MovementSys::~MovementSys()
{
}

QString MovementSys::getComponentType() const
{
    return typeid(CInputReceiver).name();
}

inline uint qHash(const std::shared_ptr<CTransform> &key) {
    return key->getId();
}

void MovementSys::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CTransform> t = c->getSibling<CTransform>();
    m_transforms.insert(t);
}

void MovementSys::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CTransform> t = c->getSibling<CTransform>();
    m_transforms.remove(t);
}

void MovementSys::tick(float seconds)
{
    std::shared_ptr<CCamera> cam = m_graphics->getActiveCamera();
    std::shared_ptr<Camera> graphicsCam = cam->getCameraObject();

    glm::vec3 forward = graphicsCam->getLook();
    forward.y = 0;
    forward = glm::normalize(forward) * MOVEMENT_SPEED;

    glm::vec3 left = glm::cross(graphicsCam->getUp(), graphicsCam->getLook());
    left.y = 0;
    left = glm::normalize(left) * MOVEMENT_SPEED;

    QSetIterator<std::shared_ptr<CTransform>> it(m_transforms);

    while (it.hasNext()) {
        std::shared_ptr<CTransform> transform = it.next();

        if (m_input->isPressed(Qt::Key_W)) {
            transform->pos += forward * seconds;
        }
        if (m_input->isPressed(Qt::Key_A)) {
            transform->pos += left * seconds;
        }
        if (m_input->isPressed(Qt::Key_S)) {
            transform->pos -= forward * seconds;
        }
        if (m_input->isPressed(Qt::Key_D)) {
            transform->pos -= left * seconds;
        }

        transform->pos.y = 0.0f;
    }
}
