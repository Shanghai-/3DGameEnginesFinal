#ifndef UIDRAWER_H
#define UIDRAWER_H

#include "UIRenderable.h"
#include "engine/components/CCamera.h"
#include "UISystem.h"

#include <QHash>

class Graphics;

class UIDrawer : public UISystem
{
public:
    UIDrawer(int priority);
    ~UIDrawer();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<UIComponent> &c);
    void removeComponent(const std::shared_ptr<UIComponent> &c);

    void draw();
    void resize(int w, int h);

private:
    Graphics *m_graphics;

    std::shared_ptr<CCamera> m_uiCam;
    QVector<std::shared_ptr<UIRenderable>> m_renderables;
    QHash<Anchor, glm::vec2> m_anchorPos;

    glm::vec3 v3(glm::vec2 in);

};

#endif // UIDRAWER_H
