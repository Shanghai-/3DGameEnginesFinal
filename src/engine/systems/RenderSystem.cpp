#include "RenderSystem.h"

#include "engine/components/CTransform.h"
#include "engine/graphics/Shape.h"
#include "engine/components/CCamera.h"
#include "engine/objectManagement/GameWorld.h"

#include <QImage>

RenderSystem::RenderSystem(int priority) :
    System(priority),
    m_skyboxTex(nullptr)
{
    m_graphics = Graphics::getGlobalInstance();
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::draw()
{

    m_graphics->setShader("default");
    std::shared_ptr<CCamera> cam = m_graphics->getActiveCamera();


    QSetIterator<std::shared_ptr<CRenderable>> it(m_renderComponents);
    while (it.hasNext()) {
        std::shared_ptr<CRenderable> r = it.next();

        auto trans = r->getParent()->getComponent<CTransform>();

        m_graphics->clearTransform();
        m_graphics->translate(trans->pos);
        m_graphics->rotate(trans->rot.z, glm::vec3(0,0,1));
        m_graphics->rotate(trans->rot.y, glm::vec3(0,1,0));
        m_graphics->rotate(trans->rot.x, glm::vec3(1,0,0));
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
        m_graphics->getActiveShader()->setUniform("time", m_gameWorld->getElapsedTime());
        m_graphics->drawShape(r->getShapeName().toStdString());
    }


    if (m_skyboxTex) {
        GLint oldCullingMode, oldDepthMode;
        glGetIntegerv(GL_CULL_FACE_MODE, &oldCullingMode);
        glGetIntegerv(GL_DEPTH_FUNC, &oldDepthMode);

        m_graphics->setShader("skybox");

        glCullFace(GL_FRONT);
        glDepthFunc(GL_LEQUAL);

        m_graphics->getActiveShader()->setTexture("skyboxTexture", *(m_skyboxTex.get()));
        m_graphics->clearTransform();
        m_graphics->translate(cam->getCameraObject()->getEye());
        m_graphics->scale(glm::vec3(10.0f));
        m_graphics->setView(cam->getCameraObject()->getView());
        m_graphics->setProjection(cam->getCameraObject()->getProjection());
        m_graphics->drawShape("sphere");
        m_graphics->clearShader();

        glCullFace(oldCullingMode);
        glDepthFunc(oldDepthMode);
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

void RenderSystem::setSkybox(std::shared_ptr<TextureCube> c)
{
    m_skyboxTex = c;
    m_skyboxTex->setTextureParams(Texture::FILTER_METHOD::LINEAR, Texture::WRAP_METHOD::CLAMP_TO_EDGE);
}
