#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "engine/frame/Screen.h"
#include "engine/objectManagement/GameWorld.h"

class Application;

class MainScreen : public Screen
{
public:
    MainScreen(Application *parent);
    ~MainScreen();

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
    void initializeGame();
    void loadGraphics();
    void loadMap();
    void initializeAudio(std::shared_ptr<GameObject> player);

    //void createStar(QString name, QStringList files, )

    Application *m_parent;

    std::unique_ptr<GameWorld> m_gw;
};

#endif // MAINSCREEN_H
