#ifndef HEALTHSCRIPT_H
#define HEALTHSCRIPT_H

#include "engine/ui/scripts/Script.h"
#include "engine/ui/UIRenderable.h"
#include "CHealth.h"

#include <QVector>

class HealthScript : public Script
{
public:
    HealthScript(std::shared_ptr<CHealth> toCount);
    ~HealthScript();

    void addHeart(std::shared_ptr<UIRenderable> h);

    void onTick(float seconds);
    void onDraw();
    void onResize(int w, int h);

private:
    std::shared_ptr<CHealth> m_healthComponent;
    QVector<std::shared_ptr<UIRenderable>> m_hearts;
};

#endif // HEALTHSCRIPT_H
