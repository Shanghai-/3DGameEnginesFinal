#ifndef STARCOUNTSCRIPT_H
#define STARCOUNTSCRIPT_H

#include "engine/ui/scripts/Script.h"
#include "engine/ui/UIRenderable.h"

class StarCountScript : public Script
{
public:
    StarCountScript(UIRenderable *toChange);
    ~StarCountScript();

    void onTick(float seconds);
    void onDraw();
    void onResize(int w, int h);

    void updateStarCount(int newCount);

private:
    UIRenderable *m_target;
};

#endif // STARCOUNTSCRIPT_H
