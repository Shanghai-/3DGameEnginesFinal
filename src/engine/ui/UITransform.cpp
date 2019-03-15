#include "UITransform.h"

UITransform::UITransform(Anchor a) :
    UITransform(a, glm::vec2(0.0f), glm::vec2(1.0f), 0.0f) {}

UITransform::UITransform(Anchor a, glm::vec2 offset) :
    UITransform(a, offset, glm::vec2(1.0f), 0.0f) {}

UITransform::UITransform(Anchor a, glm::vec2 offset, glm::vec2 scale) :
    UITransform(a, offset, scale, 0.0f) {}

UITransform::UITransform(Anchor a, glm::vec2 offset, glm::vec2 scale, float rotation) :
    m_anchor(a),
    m_offset(offset),
    m_scale(scale),
    m_rotation(rotation)
{
}

UITransform::~UITransform()
{
}

void UITransform::translate(glm::vec2 translation)
{
    m_offset += translation;
}

void UITransform::setOffset(glm::vec2 offset)
{
    m_offset = offset;
}

void UITransform::rotateBy(float angle)
{
    m_rotation += angle;
}

void UITransform::setRotation(float angle)
{
    m_rotation = angle;
}

void UITransform::setScale(glm::vec2 s)
{
    m_scale = s;
}

Anchor UITransform::getAnchor() { return m_anchor; }
glm::vec2 UITransform::getOffset() { return m_offset; }
float UITransform::getRotation() { return m_rotation; }
glm::vec2 UITransform::getScale() { return m_scale; }
