#include "StarCountScript.h"

#include "engine/graphics/Graphics.h"

StarCountScript::StarCountScript(UIRenderable *toChange) :
    m_target(toChange)
{
}

StarCountScript::~StarCountScript()
{
}

void StarCountScript::onTick(float seconds)
{
}

void StarCountScript::onDraw()
{
}

void StarCountScript::onResize(int w, int h)
{
}

void StarCountScript::updateStarCount(int newCount)
{
    Graphics *g = Graphics::getGlobalInstance();
    std::string matName = m_target->getMaterial();
    Material m = g->getMaterial(matName);
    m.textureName = "Count" + std::to_string(newCount);
    g->addMaterial(matName, m);
}
