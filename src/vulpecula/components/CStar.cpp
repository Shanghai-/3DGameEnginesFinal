#include "CStar.h"

CStar::CStar(std::shared_ptr<GameObject> parent, glm::vec3 originalPos) :
    Component(parent),
    m_originalPos(originalPos)
{
}

CStar::~CStar()
{
}

glm::vec3 CStar::getOriginalPosition()
{
    return m_originalPos;
}

