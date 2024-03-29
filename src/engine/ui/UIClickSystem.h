#ifndef UICLICKSYSTEM_H
#define UICLICKSYSTEM_H

#include "UISystem.h"

#include <QHash>
#include <QSet>

class InputManager;
class UIHitbox;

class UIClickSystem : public UISystem
{
public:
    UIClickSystem(int priority);
    ~UIClickSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<UIComponent> &c);
    void removeComponent(const std::shared_ptr<UIComponent> &c);

    void tick(float seconds);
    void resize(int w, int h);

private:
    InputManager *m_input;

    int m_height;

    QVector<std::shared_ptr<UIHitbox>> m_hitboxes;
    QHash<Anchor, glm::vec2> m_anchorPos;
    QSet<uint> m_held;
    QSet<uint> m_hovered;
};

#endif // UICLICKSYSTEM_H
