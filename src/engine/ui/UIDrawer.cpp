#include "UIDrawer.h"

#include "engine/graphics/Graphics.h"
#include "engine/graphics/Shape.h"

UIDrawer::UIDrawer(int priority) :
    UISystem(priority)
{
    m_graphics = Graphics::getGlobalInstance();
    m_uiCam = std::make_shared<CCamera>(nullptr);
    m_uiCam->getCameraObject()->setUI(true);
}

UIDrawer::~UIDrawer()
{
}

QString UIDrawer::getComponentType() const
{
    return typeid(UIRenderable).name();
}

void UIDrawer::addComponent(const std::shared_ptr<UIComponent> &c)
{
    auto s = std::dynamic_pointer_cast<UIRenderable>(c);
    m_renderables.append(s);
}

void UIDrawer::removeComponent(const std::shared_ptr<UIComponent> &c)
{
    // TODO
}

void UIDrawer::draw() {
    // Switch to UI Camera
    auto oldCam = m_graphics->getActiveCamera();
    m_graphics->setCamera(m_uiCam);

    QVectorIterator<std::shared_ptr<UIRenderable>> it(m_renderables);
    while (it.hasNext())
    {
        std::shared_ptr<UIRenderable> r = it.next();
        UITransform *t = r->getTransform();
        std::shared_ptr<UIObject> p = r->getParent();

        AABB boundBox = m_graphics->getShape(r->getShapeName())->getBounds();
        glm::vec2 bounds(boundBox.getXSize(), boundBox.getYSize());
        bounds = bounds * t->getScale();

        // Translate to the parent's anchor
        m_graphics->clearTransform();
        m_graphics->translate(v3(m_anchorPos.value(p->getAnchor())));

        // Translate relative to that anchor according to our anchor
        switch (t->getAnchor()) {
            case TOP_LEFT:
                m_graphics->translate(glm::vec3(0, -bounds.y, 0));
                break;
            case TOP_CENTER:
                m_graphics->translate(glm::vec3(-bounds.x / 2, -bounds.y, 0));
                break;
            case TOP_RIGHT:
                m_graphics->translate(glm::vec3(-bounds.x, -bounds.y, 0));
                break;
            case MID_LEFT:
                m_graphics->translate(glm::vec3(0, -bounds.y / 2, 0));
                break;
            case CENTER:
                m_graphics->translate(glm::vec3(-bounds.x / 2, -bounds.y / 2, 0));
                break;
            case MID_RIGHT:
                m_graphics->translate(glm::vec3(-bounds.x, -bounds.y / 2, 0));
                break;
            case BOT_LEFT:
                break;
            case BOT_CENTER:
                m_graphics->translate(glm::vec3(-bounds.x / 2, 0, 0));
                break;
            case BOT_RIGHT:
                m_graphics->translate(glm::vec3(-bounds.x, 0, 0));
                break;
        }

        // Translate relative to our anchor according to our offset
        m_graphics->translate(v3(t->getOffset()));

        m_graphics->rotate(t->getRotation(), glm::vec3(0,0,1));
        m_graphics->scale(v3(t->getScale()));
        m_graphics->setMaterial(r->getMaterial());
        m_graphics->drawShape(r->getShapeName());
    }

    if (oldCam != nullptr) m_graphics->setCamera(oldCam);
}

// 2D screen positions have 0,0 in the bottom left
void UIDrawer::resize(int w, int h) {
    m_anchorPos[TOP_LEFT] = glm::vec2(0, h);
    m_anchorPos[TOP_CENTER] = glm::vec2(w / 2, h);
    m_anchorPos[TOP_RIGHT] = glm::vec2(w, h);

    m_anchorPos[MID_LEFT] = glm::vec2(0, h / 2);
    m_anchorPos[CENTER] = glm::vec2(w / 2, h / 2);
    m_anchorPos[MID_RIGHT] = glm::vec2(w, h / 2);

    m_anchorPos[BOT_LEFT] = glm::vec2(0, 0);
    m_anchorPos[BOT_CENTER] = glm::vec2(w / 2, 0);
    m_anchorPos[BOT_RIGHT] = glm::vec2(w, 0);

    m_uiCam->getCameraObject()->setScreenSize(glm::vec2(w, h));
}

glm::vec3 UIDrawer::v3(glm::vec2 in) {
    return glm::vec3(in.x, in.y, 0.0f);
}
