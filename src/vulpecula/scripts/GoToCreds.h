#ifndef GOTOCREDS_H
#define GOTOCREDS_H

#include "engine/ui/scripts/Script.h"

class Application;

class GoToCreds : public Script
{
public:
    GoToCreds(Application *app, float waitTime);
    ~GoToCreds();

    void onTick(float seconds);
    void onDraw();
    void onResize(int w, int h);

private:
    Application *m_app;

    float m_waitTime;
    float m_curTime;
};

#endif // GOTOCREDS_H
