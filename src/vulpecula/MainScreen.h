#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "engine/frame/Screen.h"
#include "engine/objectManagement/GameWorld.h"
#include "engine/components/volumes/CollisionVolume.h"
#include "systems/PlayerMovementSys.h"

class Application;

class MainScreen : public Screen
{
public:
    MainScreen(Application *parent, bool isServer);
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
    enum PrefabType {
        PINE_CLUSTER_1,
        PINE_CLUSTER_2,
        PINE_CLUSTER_3,
        LONE_PINE
    };

    void initializeGame();
    void loadGraphics();
    void loadMap(std::shared_ptr<PlayerMovementSys> playSys);
    void initializeAudio(std::shared_ptr<GameObject> player);

    void loadTerrain(std::shared_ptr<PlayerMovementSys> playSys);
    void loadDecorations();
    void loadObjectives();

    void createAudioZone(std::shared_ptr<GameObject> zoneObj, QStringList files,
                         std::shared_ptr<CollisionVolume> vol);
    void createStar(std::shared_ptr<GameObject> starObj, QString file, std::shared_ptr<GameObject> zone);
    void createPrefab(PrefabType type, glm::vec3 position, glm::vec3 rotationDeg, glm::vec3 scale);

    bool m_isServer;

    Application *m_parent;

    std::shared_ptr<GameWorld> m_gw;
};

#endif // MAINSCREEN_H
