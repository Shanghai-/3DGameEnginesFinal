#ifndef UISCRIPTCOMP_H
#define UISCRIPTCOMP_H

#include "UIComponent.h"
#include "Script.h"

class UIScriptComp : public UIComponent
{
public:
    UIScriptComp(std::shared_ptr<UIObject> parent, std::shared_ptr<Script> s) :
        UIComponent(parent), m_script(s) {}
    ~UIScriptComp() {}

    std::shared_ptr<Script> getScript() { return m_script; }

    void receipts() {}

private:
    std::shared_ptr<Script> m_script;
};

#endif // UISCRIPTCOMP_H
