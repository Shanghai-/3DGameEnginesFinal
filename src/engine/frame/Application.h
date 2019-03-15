#ifndef APPLICATION_H
#define APPLICATION_H

#include <QInputEvent>
#include <QStack>
#include <memory>

#include "Screen.h"
#include "engine/graphics/Camera.h"

class View;

/**
 * @brief The Application class represents one whole game in the engine.
 * An Application holds a collection of Screens on which the game does
 * its logic and drawing. It is responsible for delegating tick, draw,
 * etc. events to the various screens.
 *
 * Application should be subclassed for specific games, but they
 * shouldn't need to change anything other than the constructor (the
 * constructor needs to construct and push the game's initial screen).
 */
class Application
{
public:
    /**
     * @brief Application creates a new Application running in the
     *        given View.
     * @param v - a pointer to the View where this Application runs.
     */
    Application(View *v);

    /**
     * @brief getViewPtr gets a pointer to the overarching View class
     *        which is hosting the Application.
     * @return A pointer to the parent View.
     */
    View *getViewPtr();

    void tick(float seconds);
    void draw();
    void resize(int w, int h);

    /**
     * @brief pushScreen adds the given screen to the top of the Screen
     *        stack. Screens render from bottom to top, but process input
     *        from top to bottom, meaning the topmost screen always draws
     *        above all others and receives input first.
     * @param s - the Screen to add to the screen stack.
     */
    void pushScreen(std::shared_ptr<Screen> s);

    /**
     * @brief popScreen removes the topmost Screen from the Screen stack.
     *        This is used to remove temporary overlays, e.g. a pause
     *        screen.
     */
    void popScreen();

    /**
     * @brief clearScreens removes ALL Screens from the Screen stack.
     *        This is used to completely wipe the application, usually
     *        in order to display a new screen, e.g. replacing everything
     *        with a "Game Over" screen.
     */
    void clearScreens();

    // ====== INPUT HANDLERS ======
    void onKeyPressed(QKeyEvent *event);
    void onKeyHeld(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);

    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(QMouseEvent *event, int deltaX, int deltaY);
    void onScroll(QWheelEvent *event);

protected:
    std::vector<std::shared_ptr<Screen>> m_screens;

    View *m_view;

private:
    int m_width, m_height;
};

#endif // APPLICATION_H
