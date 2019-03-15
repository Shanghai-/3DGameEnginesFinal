#ifndef LANDINGSCREEN_H
#define LANDINGSCREEN_H

#include "engine/frame/Screen.h"
#include <memory>

class Application;
class GameWorld;

class LandingScreen : public Screen
{
public:
    LandingScreen(Application *parent);
    ~LandingScreen();

    void tick(float seconds);
    void draw();
    void resize(int w, int h);
    void onKeyPressed(QKeyEvent *event);
    void onKeyHeld(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(QMouseEvent *event, int deltaX, int deltaY);
    void onScroll(QWheelEvent *event);

    void onFocusGained();
    void onFocusLost();

private:
    std::shared_ptr<GameWorld> m_gw;
    Application *m_parent;

};

#endif // LANDINGSCREEN_H
