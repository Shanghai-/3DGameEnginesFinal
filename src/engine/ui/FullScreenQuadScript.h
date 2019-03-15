#ifndef FULLSCREENQUADSCRIPT_H
#define FULLSCREENQUADSCRIPT_H

#include "Script.h"
#include "UIRenderable.h"

class FullScreenQuadScript : public Script
{
public:
    FullScreenQuadScript(std::shared_ptr<UIRenderable> to_resize);
    ~FullScreenQuadScript();

    void onTick(float seconds);
    void onDraw();
    void onResize(int w, int h);

private:
    std::shared_ptr<UIRenderable> m_targetQuad;
};

#endif // FULLSCREENQUADSCRIPT_H
