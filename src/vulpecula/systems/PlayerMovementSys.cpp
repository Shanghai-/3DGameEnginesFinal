#include "PlayerMovementSys.h"

#include "engine/components/CInputReceiver.h"
#include "engine/components/CCamera.h"
#include "engine/graphics/Camera.h"

PlayerMovementSys::PlayerMovementSys(int priority) :
    System(priority)
{
    m_input = InputManager::getGlobalInstance();
    m_graphics = Graphics::getGlobalInstance();
}

PlayerMovementSys::~PlayerMovementSys()
{
}


QString PlayerMovementSys::getComponentType() const
{
    return typeid(CInputReceiver).name();
}

inline uint qHash(const std::shared_ptr<CTransform> &key) {
    return key->getId();
}

void PlayerMovementSys::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CTransform> t = c->getSibling<CTransform>();
    m_transforms.insert(t);
}

void PlayerMovementSys::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CTransform> t = c->getSibling<CTransform>();
    m_transforms.remove(t);
}

void PlayerMovementSys::tick(float seconds)
{
    std::shared_ptr<CCamera> cam = m_graphics->getActiveCamera();
    std::shared_ptr<Camera> graphicsCam = cam->getCameraObject();


    glm::vec3 forward = graphicsCam->getLook();
    forward.y = 0;
    forward = glm::normalize(forward) * 3.0f;

    glm::vec3 left = glm::cross(graphicsCam->getUp(), graphicsCam->getLook());
    left.y = 0;
    left = glm::normalize(left) * 3.0f;

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
