#ifndef VIEW_H
#define VIEW_H

#include "engine/util/CommonIncludes.h"
#include "engine/frame/Application.h"
#include "engine/input/InputManager.h"

#include <QGLWidget>
#include <QTime>
#include <QTimer>
#include <memory>

#include "warmup1/WarmupApp.h"
#include "dungeon_crawler/DungeonCrawlerApp.h"

class Graphics;

/**
 * This is similar to your "CS1971FrontEnd" class. Here you will receive all of the input events
 * to forward to your game.
 *
 * @brief The View class
 */
class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

    void captureMouse(bool b);

private:
    static const int FRAMES_TO_AVERAGE = 30;

private:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyRepeatEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QWidget *m_window;
    QTime m_time;
    QTimer m_timer;
    bool m_captureMouse;

    float m_fps;
    int m_frameIndex;
    float m_frameTimes[FRAMES_TO_AVERAGE];

    Graphics* m_graphics;
    InputManager *m_inManager;

    //std::unique_ptr<WarmupApp> m_app;
    std::unique_ptr<DungeonCrawlerApp> m_app;

private slots:
    void tick();
};

#endif // VIEW_H

