#include "UIText.h"

UIText::UIText(std::shared_ptr<UIObject> parent, UITransform t, QString text) :
    UIText(parent, t, text, 48.0f, "default", "default")
{
}

UIText::UIText(std::shared_ptr<UIObject> parent, UITransform t, QString text, float size) :
    UIText(parent, t, text, size, "default", "default")
{
}

UIText::UIText(std::shared_ptr<UIObject> parent, UITransform t, QString text, float size, QString material) :
    UIText(parent, t, text, size, material, "default")
{
}

UIText::UIText(std::shared_ptr<UIObject> parent, UITransform t,
               QString text, float size, QString material, QString fontName) :
    UIComponent(parent),
    m_transform(t),
    m_text(text),
    m_charSize(size),
    m_material(material),
    m_font(fontName)
{
}

UIText::~UIText()
{
}

UITransform UIText::getTransform() { return m_transform; }
std::string UIText::getText() { return m_text.toStdString(); }
float UIText::getSize() { return m_charSize; }
std::string UIText::getMaterial() { return m_material.toStdString(); }
std::string UIText::getFontName() { return m_font.toStdString(); }
void UIText::setText(QString t) { m_text = t; }

void UIText::receipts()
{
}
