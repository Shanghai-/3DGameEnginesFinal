#ifndef WARMUPSCREEN_H
#define WARMUPSCREEN_H

#include <memory>

#include "engine/frame/Screen.h"

class GameWorld;
class Application;

class WarmupScreen : public Screen
{
public:
    WarmupScreen(Application *parent);
    ~WarmupScreen();

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

#endif // WARMUPSCREEN_H
