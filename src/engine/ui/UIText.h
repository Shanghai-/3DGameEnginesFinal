#ifndef UITEXT_H
#define UITEXT_H

#include "UIComponent.h"
#include "UITransform.h"

class UIText : public UIComponent
{
public:
    UIText(std::shared_ptr<UIObject> parent, UITransform t, QString text);
    UIText(std::shared_ptr<UIObject> parent, UITransform t, QString text, float size);
    UIText(std::shared_ptr<UIObject> parent, UITransform t, QString text, float size, QString material);
    UIText(std::shared_ptr<UIObject> parent, UITransform t,
           QString text, float size, QString material, QString fontName);
    ~UIText();

    UITransform getTransform();
    std::string getText();
    float getSize();
    std::string getMaterial();
    std::string getFontName();

    void setText(QString t);

private:
    UITransform m_transform;
    QString m_text;
    float m_charSize;
    QString m_material;
    QString m_font;

    // UIComponent interface
public:
    void receipts();
};

#endif // UITEXT_H
