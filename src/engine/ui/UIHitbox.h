#ifndef UIHITBOX_H
#define UIHITBOX_H

#include "UIComponent.h"
#include "UITransform.h"
#include "ClickResponse.h"

class UIHitbox : public UIComponent
{
public:
    UIHitbox(std::shared_ptr<UIObject> parent, UITransform t, ClickResponse *r) :
        UIComponent(parent), m_transform(t), m_response(r) {}
    ~UIHitbox() { delete m_response; }

    UITransform getTransform() { return m_transform; }
    ClickResponse *getResponse() { return m_response; }

    void receipts() {}

private:
    UITransform m_transform;
    ClickResponse *m_response;
};

#endif // UIHITBOX_H
