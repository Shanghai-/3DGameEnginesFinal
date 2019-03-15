#ifndef FPSCAMERASYSTEM_H
#define FPSCAMERASYSTEM_H

#include "CameraSystem.h"
#include "engine/input/InputManager.h"

class FPSCameraSystem : public CameraSystem
{
public:
    FPSCameraSystem(int priority);
    ~FPSCameraSystem();

    void updateCamera(std::shared_ptr<CCamera> camComp,
                      std::shared_ptr<Camera> graphicsCam);

private:
    InputManager *m_input;
};

#endif // FPSCAMERASYSTEM_H
