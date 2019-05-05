#include "TimedFade.h"

#include "engine/graphics/Graphics.h"

#include <iostream>

TimedFade::TimedFade(UIRenderable *renderable, float fadeTimeSeconds) :
    m_renderable(renderable),
    m_accumulatedSeconds(0.0f),
    m_targetSeconds(fadeTimeSeconds)
{
    m_graphics = Graphics::getGlobalInstance();
}

TimedFade::~TimedFade()
{
}

void TimedFade::onTick(float seconds)
{
    if (m_accumulatedSeconds > m_targetSeconds) return;

    m_accumulatedSeconds += seconds;

    float progress = m_accumulatedSeconds / m_targetSeconds;
    progress = glm::clamp(progress, 0.0f, 1.0f);

    std::string matName = m_renderable->getMaterial();
    Material m = m_graphics->getMaterial(matName);
    m.alpha = progress;
    m_graphics->addMaterial(matName, m);
}


void TimedFade::onDraw()
{
}

void TimedFade::onResize(int w, int h)
{
}
