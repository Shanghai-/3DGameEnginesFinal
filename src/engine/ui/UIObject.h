#ifndef UIOBJECT_H
#define UIOBJECT_H

#include <QString>
#include <QVector>
#include <memory>

enum Anchor {
    TOP_LEFT, TOP_CENTER, TOP_RIGHT,
    MID_LEFT, CENTER, MID_RIGHT,
    BOT_LEFT, BOT_CENTER, BOT_RIGHT
};

class UIComponent;

class UIObject
{
public:
    UIObject(QString name, const uint id, Anchor anchor_pos);
    ~UIObject();

    void addComponent(std::shared_ptr<UIComponent> c);

    QVector<std::shared_ptr<UIComponent>> getComponents();
    Anchor getAnchor();
    QString getName();

private:
    QString m_name;
    uint m_id;
    Anchor m_pos;

    QVector<std::shared_ptr<UIComponent>> m_components;
};

#endif // UIOBJECT_H
