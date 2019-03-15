#include "CameraSystem.h"

CameraSystem::CameraSystem(int priority) :
    System(priority)
{
    m_graphics = Graphics::getGlobalInstance();
}

CameraSystem::~CameraSystem()
{
}

QString CameraSystem::getComponentType() const
{
    return typeid(CCamera).name();
}

inline uint qHash(const std::shared_ptr<CCamera> &key) {
    return key->getId();
}

void CameraSystem::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CCamera> recast = std::dynamic_pointer_cast<CCamera>(c);
    m_cameras.insert(recast);

    if (m_graphics->getActiveCamera() == nullptr && recast->isActive()) {
        m_graphics->setCamera(recast);
    }
}

void CameraSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CCamera> recast = std::dynamic_pointer_cast<CCamera>(c);
    m_cameras.remove(recast);
}

void CameraSystem::tick(float seconds)
{
    QSetIterator<std::shared_ptr<CCamera>> it(m_cameras);
    while (it.hasNext()) {
        std::shared_ptr<CCamera> camComp = it.next();
        std::shared_ptr<Camera> graphicsCam = camComp->getCameraObject();

        graphicsCam->setScreenSize(m_screenSize);

        if (camComp->isActive()) {
            m_graphics->setCamera(camComp);
        }

        updateCamera(camComp, graphicsCam);

        glm::mat4 frustum = graphicsCam->getProjection() * graphicsCam->getView();
        //glm::mat4 frustum = graphicsCam->getView();

        camComp->setR0(glm::vec4(frustum[0][0], frustum[1][0], frustum[2][0], frustum[3][0]));
        camComp->setR1(glm::vec4(frustum[0][1], frustum[1][1], frustum[2][1], frustum[3][1]));
        camComp->setR2(glm::vec4(frustum[0][2], frustum[1][2], frustum[2][2], frustum[3][2]));
        camComp->setR3(glm::vec4(frustum[0][3], frustum[1][3], frustum[2][3], frustum[3][3]));
    }
}

void CameraSystem::resize(int w, int h)
{
    m_screenSize = glm::vec2(w, h);
}
