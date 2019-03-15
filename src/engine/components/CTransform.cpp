#include "CTransform.h"

CTransform::CTransform(std::shared_ptr<GameObject> parent, bool unmoving) :
    CTransform(parent, unmoving,
               glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
{
}

CTransform::CTransform(std::shared_ptr<GameObject> parent, bool unmoving, glm::vec3 pos) :
    CTransform(parent, unmoving,
               pos, glm::vec3(0.0f), glm::vec3(1.0f))
{
}

CTransform::CTransform(std::shared_ptr<GameObject> parent, bool unmoving,
                       glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) :
    Component(parent),
    m_static(unmoving),
    pos(pos),
    rot(rot),
    scale(scale)
{
}

CTransform::~CTransform()
{
}

bool CTransform::is_static() const { return m_static; }
QString CTransform::getRequiredSystem() { return "None"; }

void CTransform::translate(glm::vec3 translation)
{
    pos += translation;
}

void CTransform::rotate(glm::vec3 rotation)
{
    setRotation(rot + rotation);
}

void CTransform::scaleBy(glm::vec3 s)
{
    setScale(scale * s);
}

void CTransform::setRotation(glm::vec3 rotation)
{
    rot = rotation;
}

void CTransform::setScale(glm::vec3 s)
{
    scale = s;
}

glm::vec3 CTransform::toWorld(glm::vec3 localPos)
{
    return localPos + pos;
}
