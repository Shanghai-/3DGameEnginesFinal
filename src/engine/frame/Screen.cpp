#include "Screen.h"

Screen::Screen() :
    m_shouldTick(true),
    m_shouldDraw(true)
{
}

// These functions indicate to the Application whether
// this specific screen should be updated/drawn in the
// update/draw step
bool Screen::shouldTick() { return m_shouldTick; }
bool Screen::shouldDraw() { return m_shouldDraw; }
