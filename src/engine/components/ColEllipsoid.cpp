#include "ColEllipsoid.h"
#include "engine/graphics/Graphics.h"
#include "engine/components/CTransform.h"

ColEllipsoid::ColEllipsoid(std::shared_ptr<GameObject> parent, glm::vec3 radii) : Component(parent),
    m_radii(radii)
{

}

ColEllipsoid::~ColEllipsoid()
{

}

void ColEllipsoid::drawWireframe()
{
    Graphics *g = Graphics::getGlobalInstance();
    auto thisTrans = getParent()->getComponent<CTransform>();

    g->clearTransform();
    g->translate(thisTrans->toWorld(glm::vec3(0)));
    glm::vec3 test = thisTrans->toWorld(glm::vec3(0));
    g->scale(m_radii * 2.f);
    g->drawShape("wire_sphere");
}

glm::vec3 ColEllipsoid::getRadii()
{
    return m_radii;
}
