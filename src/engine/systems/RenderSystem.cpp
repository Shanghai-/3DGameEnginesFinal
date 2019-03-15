#include "RenderSystem.h"

#include "engine/components/CTransform.h"
#include "engine/graphics/Shape.h"
#include "engine/components/CCamera.h"

#include <iostream>
#include "glm/gtx/string_cast.hpp"

RenderSystem::RenderSystem(int priority) :
    System(priority)
{
    m_graphics = Graphics::getGlobalInstance();
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::draw()
{
    std::shared_ptr<CCamera> cam = m_graphics->getActiveCamera();

    QSetIterator<std::shared_ptr<CRenderable>> it(m_renderComponents);
    while (it.hasNext()) {
        std::shared_ptr<CRenderable> r = it.next();

        auto trans = r->getParent()->getComponent<CTransform>();

        m_graphics->clearTransform();
        m_graphics->translate(trans->pos);
        m_graphics->rotate(trans->rot.x, glm::vec3(1,0,0));
        m_graphics->rotate(trans->rot.y, glm::vec3(0,1,0));
        m_graphics->rotate(trans->rot.z, glm::vec3(0,0,1));
        m_graphics->scale(trans->scale);

        AABB bounds = r->getShape()->getBounds();
        bounds.updateBounds(m_graphics->getTransform());
        QVector<glm::vec4> *points = bounds.getPoints();

        // Code below draws AABB bounds for debuggo
        /* m_graphics->clearTransform();
        m_graphics->translate(trans->pos);
        bounds.draw(m_graphics);
        m_graphics->clearTransform();
        m_graphics->translate(trans->pos);
        m_graphics->scale(trans->scale); */

        if (behindPlane(cam->getNegXPlane(), points)) continue;
        if (behindPlane(cam->getNegYPlane(), points)) continue;
        if (behindPlane(cam->getNegZPlane(), points)) continue;
        if (behindPlane(cam->getPosXPlane(), points)) continue;
        if (behindPlane(cam->getPosYPlane(), points)) continue;
        if (behindPlane(cam->getPosZPlane(), points)) continue;

        m_graphics->setMaterial(r->getMaterialName().toStdString());
        m_graphics->drawShape(r->getShapeName().toStdString());
    }
}

bool RenderSystem::behindPlane(glm::vec4 planeConstants, QVector<glm::vec4> *points)
{
    QVectorIterator<glm::vec4> it(*points);
    while (it.hasNext()) {
        glm::vec4 point = it.next();
        if (planeConstants.x * point.x
                + planeConstants.y * point.y
                + planeConstants.z * point.z
                + planeConstants.w
                >= 0.0f) {
            return false;
        }
    }
    return true;
}

inline uint qHash(const std::shared_ptr<CRenderable> &key) {
    return key->getId();
}

QString RenderSystem::getComponentType() const
{
    return typeid(CRenderable).name();
}

void RenderSystem::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CRenderable> r = std::dynamic_pointer_cast<CRenderable>(c);
    m_renderComponents.insert(r);
}

void RenderSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CRenderable> r = std::dynamic_pointer_cast<CRenderable>(c);
    m_renderComponents.remove(r);
}
