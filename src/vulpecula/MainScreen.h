#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "engine/frame/Screen.h"
#include "engine/objectManagement/GameWorld.h"
#include "engine/components/volumes/CollisionVolume.h"

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

    void createAudioZone(std::shared_ptr<GameObject> zoneObj, QStringList files,
                         std::shared_ptr<CollisionVolume> vol);
    void createStar(std::shared_ptr<GameObject> starObj, QString file, std::shared_ptr<GameObject> zone);

    Application *m_parent;

    std::unique_ptr<GameWorld> m_gw;
};

#endif // MAINSCREEN_H
