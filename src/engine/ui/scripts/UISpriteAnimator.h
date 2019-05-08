#ifndef UISPRITEANIMATOR_H
#define UISPRITEANIMATOR_H

#include "Script.h"
#include "QStringList"
#include "engine/ui/UIRenderable.h"
#include "engine/graphics/Graphics.h"

class UISpriteAnimator : public Script
{
public:
    UISpriteAnimator(UIRenderable *toAnimate, QStringList textureNames,
                     float timeBetweenFrames);
    ~UISpriteAnimator();

    void onTick(float seconds);
    void onDraw();
    void onResize(int w, int h);

private:
    UIRenderable *m_toAnimate;
    QStringList m_texNames;

    float m_waitTime;
    float m_curTime;
    int m_curIndex;

    Graphics *m_graphics;
};

#endif // UISPRITEANIMATOR_H
