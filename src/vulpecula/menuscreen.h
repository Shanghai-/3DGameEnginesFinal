#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include "engine/frame/Screen.h"

class Application;

class MenuScreen : public Screen
{
public:
    MenuScreen(Application* Parent);
    ~MenuScreen();

    // Screen interface
public:
    void tick(float seconds);
    void draw();
    void resize(int w, int h);
    void onFocusGained();
    void onFocusLost();
    void onKeyPressed(QKeyEvent *event);
    void onKeyHeld(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(QMouseEvent *event, int deltaX, int deltaY);
    void onScroll(QWheelEvent *event);

private:
    Application *m_parent;
};

#endif // MENUSCREEN_H
