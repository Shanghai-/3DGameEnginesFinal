#ifndef SCREEN_H
#define SCREEN_H

#include <QInputEvent>

/**
 * @brief The Screen class represents a single display of a game.
 * For example, a game might have screens for each level, a screen
 * for the pause menu, and a screen for the main menu. Screens
 * should not contain any game logic - that's what Systems are for.
 * Instead, a screen should simply instantiate a GameWorld and
 * any Systems and Components it may need to start off in its
 * constructor, then leave the rest to the GameWorld by updating
 * it every tick.
 */
class Screen
{
public:
    Screen();

    /**
     * @brief tick is called ~60 times a second. This is where all
     *        the game logic happens. HOWEVER: for Screens, there
     *        should be no game logic here; instead, pass the tick
     *        event on to any GameWorlds your screen uses.
     * @param seconds - the number of seconds that have passed
     *        since the last tick
     */
    virtual void tick(float seconds) = 0;

    /**
     * @brief draw renders the world. This should be handled by
     *        a GameWorld object, so simply pass this call on
     *        to whatever GameWorld you want to draw.
     */
    virtual void draw() = 0;

    /**
     * @brief resize is called whenever the screen resizes. This
     *        call should be passed on to the GameWorld.
     * @param w - the new width of the screen in pixels
     * @param h - the new height of the screen in pixels
     */
    virtual void resize(int w, int h) = 0;

    /**
     * @brief shouldTick tells the Application whether this screen
     *        should respond to Tick events or not at the current
     *        moment. Use this to pause screens.
     * @return true if the screen should tick, false otherwise
     */
    bool shouldTick();

    /**
     * @brief shouldDraw tells the Application whether this screen
     *        should respond to Draw calls at the current moment.
     *        Use this to stop drawing screens that are not visible.
     * @return true if the screen should draw, false otherwise
     */
    bool shouldDraw();

    /**
     * @brief onFocusGained is called by the Application when this
     * screen becomes the top member of the Screen stack. This is
     * most useful for operations like capturing/freeing the mouse
     * and setting the values of shouldTick and shouldDraw.
     */
    virtual void onFocusGained() = 0;

    /**
     * @brief onFocusLost is called by the Applicatin when this
     * screen is first covered by another on the Screen stack.
     * This is useful for capturing/freeing the mouse and setting
     * the values of shouldTick and shouldDraw.
     */
    virtual void onFocusLost() = 0;

    // We provide default empty declarations for the input tracking
    // methods, since they should almost never be used (see InputManager.h)
    virtual void onKeyPressed(QKeyEvent *event) {}
    virtual void onKeyHeld(QKeyEvent *event) {}
    virtual void onKeyReleased(QKeyEvent *event) {}

    virtual void onMousePressed(QMouseEvent *event) {}
    virtual void onMouseReleased(QMouseEvent *event) {}
    virtual void onMouseMoved(QMouseEvent *event, int deltaX, int deltaY) {}
    virtual void onScroll(QWheelEvent *event) {}

protected:
    bool m_shouldTick;
    bool m_shouldDraw;
};

#endif // SCREEN_H
