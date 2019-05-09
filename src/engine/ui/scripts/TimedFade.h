#ifndef TIMEDFADE_H
#define TIMEDFADE_H

#include "Script.h"
#include "engine/ui/UIRenderable.h"
#include "engine/ui/UIText.h"

class Graphics;

class TimedFade : public Script
{
public:
    TimedFade(UIRenderable *renderable, float fadeTimeSeconds);
    TimedFade(UIRenderable *renderable, float fadeTimeSeconds, float waitTimeSeconds);
    TimedFade(UIText *text, float fadeTimeSeconds);
    TimedFade(UIText *text, float fadeTimeSeconds, float waitTimeSeconds);
    ~TimedFade();

    void onTick(float seconds);

    void onDraw();
    void onResize(int w, int h);

private:
    UIRenderable *m_renderable;
    UIText *m_text;
    float m_accumulatedSeconds;
    float m_waitSeconds;
    float m_targetSeconds;

    Graphics *m_graphics;
};

#endif // TIMEDFADE_H
