#ifndef TEXTDRAWER_H
#define TEXTDRAWER_H

#include "UISystem.h"

#include <QHash>

class UIText;
class CCamera;
class Graphics;

class TextDrawer : public UISystem
{
public:
    TextDrawer(int priority);
    ~TextDrawer();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<UIComponent> &c);
    void removeComponent(const std::shared_ptr<UIComponent> &c);

    void draw();
    void resize(int w, int h);

private:
    Graphics *m_graphics;

    std::shared_ptr<CCamera> m_uiCam;
    QVector<std::shared_ptr<UIText>> m_texts;
    QHash<Anchor, glm::vec2> m_anchorPos;

    glm::vec3 v3(glm::vec2 in);
};

#endif // TEXTDRAWER_H
