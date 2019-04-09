#ifndef THIRDPERSONCAMSYS_H
#define THIRDPERSONCAMSYS_H

#include "CameraSystem.h"
#include "engine/input/InputManager.h"

class ThirdPersonCamSys : public CameraSystem
{
public:
    ThirdPersonCamSys(int priority, float min_zoom, float max_zoom, float initial_zoom);
    ~ThirdPersonCamSys();

    void updateCamera(std::shared_ptr<CCamera> camComp, std::shared_ptr<Camera> graphicsCam);

private:
    InputManager *m_input;

    float m_min_zoom, m_max_zoom, m_cur_zoom;
};

#endif // THIRDPERSONCAMSYS_H
