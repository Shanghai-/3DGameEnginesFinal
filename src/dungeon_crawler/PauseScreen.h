#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

#include "engine/frame/Screen.h"

#include <memory>

class GameWorld;
class Application;
class InputManager;

class PauseScreen : public Screen
{
public:
    PauseScreen(Application *parent);

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
    InputManager *m_input;
    bool m_primed;

};

#endif // PAUSESCREEN_H
