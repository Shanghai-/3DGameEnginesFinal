#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include "System.h"
#include "engine/components/CCamera.h"
#include "engine/graphics/Graphics.h"

#include "QSet"

class CameraSystem : public System
{
public:
    CameraSystem(int priority);
    ~CameraSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);
    void resize(int w, int h);

    virtual void updateCamera(std::shared_ptr<CCamera> camComp,
                              std::shared_ptr<Camera> graphicsCam) = 0;

private:
    Graphics *m_graphics;
    glm::vec2 m_screenSize;
    QSet<std::shared_ptr<CCamera>> m_cameras;
};

#endif // CAMERASYSTEM_H
