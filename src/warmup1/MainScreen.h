#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <memory>

#include "engine/frame/Screen.h"

class GameWorld;
class Application;

class MainScreen : public Screen
{
public:
    MainScreen(Application *parent);
    ~MainScreen();

    // Screen interface
    void tick(float seconds);
    void draw();
    void resize(int w, int h);
    void onKeyPressed(QKeyEvent *event);
    void onKeyHeld(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onScroll(QWheelEvent *event);
    void onMouseMoved(QMouseEvent *event, int deltaX, int deltaY);

    void onFocusGained();
    void onFocusLost();

private:
    Application *m_parent;

    std::unique_ptr<GameWorld> m_gw;

};

#endif // MAINSCREEN_H
