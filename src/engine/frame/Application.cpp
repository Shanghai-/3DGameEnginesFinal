#include "Application.h"

Application::Application(View *v) :
    m_view(v),
    m_width(1),
    m_height(1)
{
}

View *Application::getViewPtr()
{
    return m_view;
}

void Application::tick(float seconds)
{
    // On tick, we start with the bottom-most screen of the list
    // and check if it wants to update. If yes, we update it.
    for (unsigned int i = 0; i < m_screens.size(); i++)
    {
        std::shared_ptr<Screen> s = m_screens[i];
        if (s->shouldTick())
        {
            s->tick(seconds);
        }
    }
}

void Application::draw()
{
    // On draw, we also start with the bottom-most screen and
    // only draw the ones that want to be drawn. We start from the
    // bottom so that overlays can draw after the screens under them.
    for (unsigned int i = 0; i < m_screens.size(); i++)
    {
        std::shared_ptr<Screen> s = m_screens[i];
        if (s->shouldDraw())
        {
            s->draw();
        }
    }
}

void Application::resize(int w, int h)
{
    // Store width and height for resizing new Screens later
    m_width = w;
    m_height = h;

    // Resize is applied to every screen no matter what
    for (unsigned int i = 0; i < m_screens.size(); i++)
    {
        std::shared_ptr<Screen> s = m_screens[i];
        s->resize(w, h);
    }
}

void Application::pushScreen(std::shared_ptr<Screen> s) {
    // Before we push the new screen onto the top, we tell the old
    // top screen that it's losing top/focus status, which allows
    // it to do things like disable ticking or release the mouse.
    if (!m_screens.empty()) m_screens.back()->onFocusLost();

    // Add the new screen to the end (top) of the screen list
    m_screens.push_back(s);

    // Tell the new screen that it has gained focus so it can do
    // processing like above
    s->onFocusGained();

    // This is an odd one, but if we don't resize the screen, it
    // believes that the application window is just 1x1 pixels until
    // a new resize event comes in.
    s->resize(m_width, m_height);
}

void Application::popScreen() {
    assert(!m_screens.empty()); // Can't pop from empty

    // Previous top screen loses focus, then is popped
    m_screens.back()->onFocusLost();
    m_screens.pop_back();

    // If there's still screens left in the stack, the new top
    // screen gains focus.
    if (!m_screens.empty()) m_screens.back()->onFocusGained();
}

void Application::clearScreens() { m_screens.clear(); }

// Input handling functions - right now, these just forward all input to
// the topmost screen.
void Application::onKeyPressed(QKeyEvent *event) { m_screens.back()->onKeyPressed(event); }
void Application::onKeyHeld(QKeyEvent *event) { m_screens.back()->onKeyHeld(event); }
void Application::onKeyReleased(QKeyEvent *event) { m_screens.back()->onKeyReleased(event); }

void Application::onMousePressed(QMouseEvent *event) { m_screens.back()->onMousePressed(event); }
void Application::onMouseReleased(QMouseEvent *event) { m_screens.back()->onMouseReleased(event); }
void Application::onScroll(QWheelEvent *event) { m_screens.back()->onScroll(event); }
void Application::onMouseMoved(QMouseEvent *event, int deltaX, int deltaY) {
    m_screens.back()->onMouseMoved(event, deltaX, deltaY);
}

