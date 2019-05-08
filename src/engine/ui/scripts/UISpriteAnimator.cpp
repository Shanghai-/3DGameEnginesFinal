#include "UISpriteAnimator.h"

UISpriteAnimator::UISpriteAnimator(UIRenderable *toAnimate, QStringList textureNames,
                                   float timeBetweenFrames) :
    m_toAnimate(toAnimate),
    m_texNames(textureNames),
    m_waitTime(timeBetweenFrames),
    m_curTime(0.0f),
    m_curIndex(0)
{
    m_graphics = Graphics::getGlobalInstance();
}

UISpriteAnimator::~UISpriteAnimator()
{
}

void UISpriteAnimator::onTick(float seconds)
{
    m_curTime += seconds;

    if (m_curTime >= m_waitTime) {
        m_curIndex++;
        if (m_curIndex >= m_texNames.length()) m_curIndex = 0;

        m_curTime = m_waitTime - m_curTime;

        QString nextTex = m_texNames.at(m_curIndex);
        std::string matName = m_toAnimate->getMaterial();
        Material m = m_graphics->getMaterial(matName);
        m.textureName = nextTex.toStdString();
        m_graphics->addMaterial(matName, m);
    }
}

void UISpriteAnimator::onDraw()
{
}

void UISpriteAnimator::onResize(int w, int h)
{
}
