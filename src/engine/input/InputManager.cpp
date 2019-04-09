#include "InputManager.h"

InputManager::InputManager() :
    m_mousePos(glm::vec2(0.0f)),
    m_mouseDelta(glm::vec2(0.0f)),
    m_scrollAngle(0.0f)
{
}

InputManager::~InputManager()
{
}

bool InputManager::isPressed(Qt::Key k)
{
    return m_keys.value(k, KEY_STATE::RELEASED) == KEY_STATE::PRESSED;
}

bool InputManager::isPressed(Qt::MouseButton b)
{
    return m_buttons.value(b, KEY_STATE::RELEASED) == KEY_STATE::PRESSED;
}

glm::vec2 InputManager::getMouseDelta()
{
    return m_mouseDelta;
}

void InputManager::resetMouseDelta()
{
    m_mouseDelta = glm::vec2(0.f);
}

glm::vec2 InputManager::getMousePos()
{
    return m_mousePos;
}

float InputManager::getScrollAngle()
{
    return m_scrollAngle;
}

// After reading the scroll angle, the engine user may want to
// react after it reaches a certain angle and then reset it so
// that they can re-start that 0 to target angle loop process
void InputManager::resetScrollAngle()
{
    m_scrollAngle = 0.f;
}

/* ========================== *\
 * Input event handling below *
\* ========================== */

void InputManager::onKeyPressed(QKeyEvent *event)
{
    m_keys[event->key()] = KEY_STATE::PRESSED;
}

void InputManager::onKeyReleased(QKeyEvent *event)
{
    m_keys[event->key()] = KEY_STATE::RELEASED;
}

void InputManager::onMousePressed(QMouseEvent *event)
{
    m_buttons[event->button()] = KEY_STATE::PRESSED;
}

void InputManager::onMouseReleased(QMouseEvent *event)
{
    m_buttons[event->button()] = KEY_STATE::RELEASED;
}

void InputManager::onMouseMoved(QMouseEvent *event, int deltaX, int deltaY)
{
    m_mousePos = glm::vec2(event->localPos().x(), event->localPos().y());
    m_mouseDelta = glm::vec2(deltaX, deltaY);
}

void InputManager::onScroll(QWheelEvent *event)
{
    m_scrollAngle += event->angleDelta().y() / 8.f; // Dividing by 8 puts things in terms of degrees
}
