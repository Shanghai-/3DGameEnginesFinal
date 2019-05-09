#include "FullScreenQuadScript.h"

#include <iostream>

FullScreenQuadScript::FullScreenQuadScript(std::shared_ptr<UIRenderable> to_resize) :
    m_targetQuad(to_resize)
{
}

FullScreenQuadScript::~FullScreenQuadScript()
{
}

void FullScreenQuadScript::onTick(float seconds)
{
}

void FullScreenQuadScript::onDraw()
{
}

void FullScreenQuadScript::onResize(int w, int h)
{
    m_targetQuad->getTransform()->setScale(glm::vec2(w, h));
}
