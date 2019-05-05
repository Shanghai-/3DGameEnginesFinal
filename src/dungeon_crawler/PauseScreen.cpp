#include "PauseScreen.h"

#include "engine/objectManagement/GameWorld.h"
#include "engine/graphics/Graphics.h"

#include "engine/ui/UIDrawer.h"
#include "engine/ui/TextDrawer.h"
#include "engine/ui/UIObject.h"
#include "engine/ui/UIRenderable.h"
#include "engine/ui/UIText.h"
#include "engine/ui/UIScriptSys.h"
#include "engine/ui/UIScriptComp.h"
#include "engine/ui/scripts/FullScreenQuadScript.h"
#include "engine/input/InputManager.h"

#include "engine/frame/Application.h"
#include "view.h"

PauseScreen::PauseScreen(Application *parent) :
    m_parent(parent),
    m_primed(false)
{
    // Set up graphics
    Graphics *graphics = Graphics::getGlobalInstance();

    // Gray
    Material m;
    m.shaderName = "default";
    m.color = glm::vec3(0.3);
    m.useLighting = false;
    m.alpha = 0.6f;
    graphics->addMaterial("gray", m);

    m_gw = std::make_shared<GameWorld>();

    m_gw->registerUISystem(std::make_shared<UIScriptSys>(100));
    m_gw->registerUISystem(std::make_shared<UIDrawer>(200));
    m_gw->registerUISystem(std::make_shared<TextDrawer>(300));

    std::shared_ptr<UIObject> text = std::make_shared<UIObject>("text", m_gw->getNewObjID(), CENTER);
    UITransform t = UITransform(CENTER);
    text->addComponent(std::make_shared<UIText>(text, t, "- PAUSED -", 128, "white"));
    m_gw->addUIObject(text);

    std::shared_ptr<UIObject> bg = std::make_shared<UIObject>("bg", m_gw->getNewObjID(), BOT_LEFT);
    t = UITransform(BOT_LEFT);
    auto c = std::make_shared<UIRenderable>(bg, t, "uiquad", "gray");
    auto s = std::make_shared<FullScreenQuadScript>(c);
    bg->addComponent(c);
    bg->addComponent(std::make_shared<UIScriptComp>(bg, s));
    m_gw->addUIObject(bg);

    m_input = InputManager::getGlobalInstance();
}

void PauseScreen::tick(float seconds)
{
    m_gw->tick(seconds);

    if (m_input->isPressed(Qt::Key_P)) {
        if (m_primed) m_parent->popScreen();
    } else {
        m_primed = true;
    }
}

void PauseScreen::draw()
{
    m_gw->draw();
}

void PauseScreen::resize(int w, int h)
{
    m_gw->resize(w, h);
}

void PauseScreen::onKeyPressed(QKeyEvent *event) {}
void PauseScreen::onKeyHeld(QKeyEvent *event) {}
void PauseScreen::onKeyReleased(QKeyEvent *event) {}
void PauseScreen::onMousePressed(QMouseEvent *event) {}
void PauseScreen::onMouseReleased(QMouseEvent *event) {}
void PauseScreen::onMouseMoved(QMouseEvent *event, int deltaX, int deltaY) {}
void PauseScreen::onScroll(QWheelEvent *event) {}


void PauseScreen::onFocusGained()
{
    m_parent->getViewPtr()->captureMouse(false);
    m_shouldTick = true;
    m_shouldDraw = true;
}

void PauseScreen::onFocusLost()
{
}
