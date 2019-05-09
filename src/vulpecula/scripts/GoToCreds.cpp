#include "GoToCreds.h"

#include "engine/frame/Application.h"
#include "engine/systems/AudioSystem.h"
#include "vulpecula/CreditsScreen.h"

GoToCreds::GoToCreds(Application *app, float waitTime) :
    m_app(app),
    m_waitTime(waitTime),
    m_curTime(0.0f)
{
}

GoToCreds::~GoToCreds()
{

}

void GoToCreds::onTick(float seconds)
{
    m_curTime += seconds;

    if (m_curTime > m_waitTime) {
        m_app->popScreen();
        m_app->pushScreen(std::make_shared<CreditsScreen>(m_app));
    }
}

void GoToCreds::onDraw()
{
}

void GoToCreds::onResize(int w, int h)
{
}
