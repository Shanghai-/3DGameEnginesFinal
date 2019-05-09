#ifndef CREDITSSCREEN_H
#define CREDITSSCREEN_H

#include "engine/frame/Screen.h"
#include "engine/objectManagement/GameWorld.h"

class Application;

class CreditsScreen : public Screen
{
public:
    CreditsScreen(Application *parent);
    ~CreditsScreen();

    void tick(float seconds);
    void draw();
    void resize(int w, int h);
    void onFocusGained();
    void onFocusLost();

private:
    Application *m_parent;
    std::unique_ptr<GameWorld> m_gw;
};

#endif // CREDITSSCREEN_H
