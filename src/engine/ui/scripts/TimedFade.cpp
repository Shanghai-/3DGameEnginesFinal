#include "TimedFade.h"

#include "engine/graphics/Graphics.h"

#include <iostream>

TimedFade::TimedFade(UIRenderable *renderable, float fadeTimeSeconds) :
    TimedFade(renderable, fadeTimeSeconds, 0.0f)
{
}

TimedFade::TimedFade(UIRenderable *renderable, float fadeTimeSeconds, float waitTimeSeconds) :
    m_renderable(renderable),
    m_text(nullptr),
    m_accumulatedSeconds(0.0f),
    m_waitSeconds(waitTimeSeconds),
    m_targetSeconds(fadeTimeSeconds)
{
    m_graphics = Graphics::getGlobalInstance();
}

TimedFade::TimedFade(UIText *text, float fadeTimeSeconds) :
    TimedFade(text, fadeTimeSeconds, 0.0f)
{
}

TimedFade::TimedFade(UIText *text, float fadeTimeSeconds, float waitTimeSeconds) :
    m_renderable(nullptr),
    m_text(text),
    m_accumulatedSeconds(0.0f),
    m_waitSeconds(waitTimeSeconds),
    m_targetSeconds(fadeTimeSeconds)
{
    m_graphics = Graphics::getGlobalInstance();
}

TimedFade::~TimedFade()
{
}

void TimedFade::onTick(float seconds)
{
    if (m_accumulatedSeconds - m_waitSeconds > m_targetSeconds) return;

    m_accumulatedSeconds += seconds;

    float progress = (m_accumulatedSeconds - m_waitSeconds) / m_targetSeconds;
    progress = glm::clamp(progress, 0.0f, 1.0f);

    std::string matName = (m_renderable == nullptr) ? m_text->getMaterial() : m_renderable->getMaterial();
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
