#include "UIScriptSys.h"

#include "UIScriptComp.h"

UIScriptSys::UIScriptSys(int priority) :
    UISystem(priority)
{
}

UIScriptSys::~UIScriptSys()
{
}

QString UIScriptSys::getComponentType() const
{
    return typeid(UIScriptComp).name();
}

void UIScriptSys::addComponent(const std::shared_ptr<UIComponent> &c)
{
    auto b = std::dynamic_pointer_cast<UIScriptComp>(c);
    m_scripts.append(b);
}

void UIScriptSys::removeComponent(const std::shared_ptr<UIComponent> &c)
{
    // TODO
}

void UIScriptSys::tick(float seconds)
{
    QVectorIterator<std::shared_ptr<UIScriptComp>> it(m_scripts);
    while (it.hasNext()) {
        it.next()->getScript()->onTick(seconds);
    }
}

void UIScriptSys::draw()
{
    QVectorIterator<std::shared_ptr<UIScriptComp>> it(m_scripts);
    while (it.hasNext()) {
        it.next()->getScript()->onDraw();
    }
}

void UIScriptSys::resize(int w, int h)
{
    QVectorIterator<std::shared_ptr<UIScriptComp>> it(m_scripts);
    while (it.hasNext()) {
        it.next()->getScript()->onResize(w, h);
    }
}
