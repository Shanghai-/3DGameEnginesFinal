#ifndef TIMEDFADE_H
#define TIMEDFADE_H

#include "Script.h"
#include "engine/ui/UIRenderable.h"

class Graphics;

class TimedFade : public Script
{
public:
    TimedFade(UIRenderable *renderable, float fadeTimeSeconds);
    ~TimedFade();

    void onTick(float seconds);

    void onDraw();
    void onResize(int w, int h);

private:
    UIRenderable *m_renderable;
    float m_accumulatedSeconds;
    float m_targetSeconds;

    Graphics *m_graphics;
};

#endif // TIMEDFADE_H
