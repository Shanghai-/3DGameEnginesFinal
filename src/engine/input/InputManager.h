#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QHash>
#include <QInputEvent>
#include <memory>

#include "glm/glm.hpp"

/**
 * @brief The InputManager class abstracts out the details of
 * input handling. It is a singleton class that maintains a
 * mapping of keyboard keys to state (currently either pressed
 * or released, but could be expanded to include held). Any
 * class that needs to process input should use InputManager's
 * public accessor methods rather than attempting to track
 * input events themselves.
 */
class InputManager
{
public:
    /**
     * @brief InputManager constructor. Internal only - call
     * getGlobalInstance() instead to get the singleton instance.
     */
    InputManager();
    ~InputManager();

    /**
     * @brief isPressed tells whether the given key is
     *        currently pressed down or not
     * @param k  - the key to check the status of
     * @return true if pressed down, false if not
     */
    bool isPressed(Qt::Key k);

    /**
     * @brief isPressed tells whether the given mouse button
     *        is currently pressed down or not
     * @param b - the button to check the status of
     * @return true if pressed down, false if not
     */
    bool isPressed(Qt::MouseButton b);

    /**
     * @brief getMousePos gets the position of the mouse in
     *        pixels relative to the top left of the window.
     * @return a vec2 of (x, y) in pixels
     */
    glm::vec2 getMousePos();

    /**
     * @brief getMouseDelta tells how much the mouse has
     *        moved between the previous reset and the
     *        current time. Typically this is reset and
     *        used every frame to detect small movements.
     * @return a vec2 of (deltaX, deltaY) in pixels
     */
    glm::vec2 getMouseDelta();

    /**
     * @brief resetMouseDelta returns the internal delta
     * accumulator to 0. Call each frame to use delta as
     * a per-frame mouse motion indicator.
     */
    void resetMouseDelta();

    /**
     * @brief getScrollAngle returns a float indicating the
     *        angle the scroll wheel has turned since the
     *        last reset. Functions similarly to getMouseDelta()
     *        in that it accumulates until it is reset.
     * @return a float representing change in angle as degrees.
     *         Typically, one notch/click on a wheel is 15
     *         degrees.
     */
    float getScrollAngle();

    /**
     * @brief resetScrollAngle returns the scroll angle
     *        accumulator to 0.
     */
    void resetScrollAngle();

    // ====== INPUT HANDLERS ======
    // These are called by View in response to input events
    // and should not be used elsewhere.
    void onKeyPressed(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(QMouseEvent *event, int deltaX, int deltaY);
    void onScroll(QWheelEvent *event);

    /**
     * @brief getGlobalInstance implements the singleton pattern
     *        for InputManager, returning a pointer to the global
     *        instance.
     * @return A raw pointer to the global InputManager instance.
     */
    static InputManager* getGlobalInstance() {
        static std::shared_ptr<InputManager> instance = std::make_shared<InputManager>();
        return instance.get();
    }

private:
    enum class KEY_STATE {
        PRESSED,
        RELEASED
    };

    // Maps keys to states
    QHash<int, KEY_STATE> m_keys;

    // Maps mouse buttons to states
    QHash<Qt::MouseButton, KEY_STATE> m_buttons;

    // Tracks current mouse position and delta
    glm::vec2 m_mousePos, m_mouseDelta;

    // Tracks scroll delta
    float m_scrollAngle;
};

#endif // INPUTMANAGER_H
