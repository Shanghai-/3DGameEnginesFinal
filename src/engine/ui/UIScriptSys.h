#ifndef UISCRIPTSYS_H
#define UISCRIPTSYS_H

#include "UISystem.h"

#include <QVector>

class UIScriptComp;

class UIScriptSys : public UISystem
{
public:
    UIScriptSys(int priority);
    ~UIScriptSys();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<UIComponent> &c);
    void removeComponent(const std::shared_ptr<UIComponent> &c);

    void tick(float seconds);
    void draw();
    void resize(int w, int h);

private:
    QVector<std::shared_ptr<UIScriptComp>> m_scripts;

    int m_width, m_height;
};

#endif // UISCRIPTSYS_H
