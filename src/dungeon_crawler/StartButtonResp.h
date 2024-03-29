#ifndef STARTBUTTONRESP_H
#define STARTBUTTONRESP_H

#include "engine/ui/ClickResponse.h"

class Application;

class StartButtonResp : public ClickResponse
{
public:
    StartButtonResp(Application *app);
    ~StartButtonResp();

    void onEnter();
    void onHover();
    void onExit();
    void onPress();
    void onHold();
    void onRelease();

private:
    Application *m_app;
};

#endif // STARTBUTTONRESP_H
