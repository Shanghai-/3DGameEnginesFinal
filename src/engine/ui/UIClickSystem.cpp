#include "UIClickSystem.h"

#include "UIHitbox.h"
#include "engine/input/InputManager.h"

UIClickSystem::UIClickSystem(int priority) :
    UISystem(priority)
{
    m_input = InputManager::getGlobalInstance();
}

UIClickSystem::~UIClickSystem()
{
}

QString UIClickSystem::getComponentType() const
{
    return typeid(UIHitbox).name();
}

void UIClickSystem::addComponent(const std::shared_ptr<UIComponent> &c)
{
    auto h = std::dynamic_pointer_cast<UIHitbox>(c);
    m_hitboxes.append(h);
}

void UIClickSystem::removeComponent(const std::shared_ptr<UIComponent> &c)
{
    // TODO
}

void UIClickSystem::tick(float seconds)
{
    bool mouseDown = m_input->isPressed(Qt::LeftButton);
    glm::vec2 mouse = m_input->getMousePos();

    for (int i = 0; i < m_hitboxes.size(); i++) {
        auto hb = m_hitboxes[i];
        UITransform t = hb->getTransform();
        auto p = hb->getParent();

        glm::vec2 anchorPos = m_anchorPos.value(p->getAnchor());
        glm::vec2 botLeft = anchorPos;
        glm::vec2 topRight = anchorPos;
        glm::vec2 bounds = t.getScale();
        glm::vec2 half_bounds = bounds / 2.0f;

        // Translate relative to that anchor according to our anchor
        switch (t.getAnchor()) {
            case TOP_LEFT:
                botLeft.y -= bounds.y;
                topRight.x += bounds.x;
                break;
            case TOP_CENTER:
                botLeft.y -= bounds.y;
                botLeft.x -= half_bounds.x;
                topRight.x += half_bounds.x;
                break;
            case TOP_RIGHT:
                botLeft.y -= bounds.y;
                botLeft.x -= bounds.x;
                break;
            case MID_LEFT:
                botLeft.y -= half_bounds.y;
                topRight.y += half_bounds.y;
                topRight.x += bounds.x;
                break;
            case CENTER:
                botLeft -= half_bounds;
                topRight += half_bounds;
                break;
            case MID_RIGHT:
                botLeft.y -= half_bounds.y;
                botLeft.x -= bounds.x;
                topRight.y += half_bounds.y;
                break;
            case BOT_LEFT:
                topRight += bounds;
                break;
            case BOT_CENTER:
                botLeft.x -= half_bounds.x;
                topRight.y += bounds.y;
                topRight.x += half_bounds.x;
                break;
            case BOT_RIGHT:
                botLeft.x -= bounds.x;
                topRight.y += bounds.y;
                break;
        }

        botLeft -= t.getOffset();
        topRight -= t.getOffset();

        if (mouse.x >= botLeft.x && mouse.x <= topRight.x
                && mouse.y >= botLeft.y && mouse.y <= topRight.y)
        {
            if (m_held.contains(i)) {
                if (mouseDown) {
                    hb->getResponse()->onHold();
                } else {
                    m_held.remove(i);
                    hb->getResponse()->onRelease();
                }
            } else {
                if (mouseDown) {
                    m_held.insert(i);
                    hb->getResponse()->onPress();
                } else {
                    hb->getResponse()->onHover();
                }
            }
        }
    }
}

void UIClickSystem::resize(int w, int h)
{
    m_anchorPos[TOP_LEFT] = glm::vec2(0, h);
    m_anchorPos[TOP_CENTER] = glm::vec2(w / 2, h);
    m_anchorPos[TOP_RIGHT] = glm::vec2(w, h);

    m_anchorPos[MID_LEFT] = glm::vec2(0, h / 2);
    m_anchorPos[CENTER] = glm::vec2(w / 2, h / 2);
    m_anchorPos[MID_RIGHT] = glm::vec2(w, h / 2);

    m_anchorPos[BOT_LEFT] = glm::vec2(0, 0);
    m_anchorPos[BOT_CENTER] = glm::vec2(w / 2, 0);
    m_anchorPos[BOT_RIGHT] = glm::vec2(w, 0);
}
