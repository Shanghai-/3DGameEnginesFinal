#ifndef UITRANSFORM_H
#define UITRANSFORM_H

#include "UIObject.h"
#include "glm/glm.hpp"

class UITransform
{
public:
    UITransform(Anchor a);
    UITransform(Anchor a, glm::vec2 offset);
    UITransform(Anchor a, glm::vec2 offset, glm::vec2 scale);
    UITransform(Anchor a, glm::vec2 offset, glm::vec2 scale, float rotation);
    ~UITransform();

    void translate(glm::vec2 translation);
    void setOffset(glm::vec2 offset);
    void rotateBy(float angle);
    void setRotation(float angle);
    void setScale(glm::vec2 s);

    Anchor getAnchor();
    glm::vec2 getOffset();
    float getRotation();
    glm::vec2 getScale();

private:
    Anchor m_anchor;
    glm::vec2 m_offset;
    float m_rotation;
    glm::vec2 m_scale;
};

#endif // UITRANSFORM_H
