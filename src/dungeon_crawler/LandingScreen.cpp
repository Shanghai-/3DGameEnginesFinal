#include "LandingScreen.h"

#include "engine/objectManagement/GameWorld.h"
#include "engine/graphics/Graphics.h"

#include "engine/ui/UIDrawer.h"
#include "engine/ui/TextDrawer.h"
#include "engine/ui/UIClickSystem.h"
#include "engine/ui/UIObject.h"
#include "engine/ui/UIHitbox.h"
#include "engine/ui/UIRenderable.h"
#include "engine/ui/UIText.h"

#include "StartButtonResp.h"

LandingScreen::LandingScreen(Application *parent) :
    m_parent(parent)
{
    // Set up graphics
    Graphics *graphics = Graphics::getGlobalInstance();
    graphics->addTexture("title", ":/images/title.png");

    // Title logo
    Material m;
    m.shaderName = "default";
    m.textureName = "title";
    m.useLighting = false;
    graphics->addMaterial("title", m);

    // Pure white (for UI)
    Material white;
    white.shaderName = "default";
    white.color = glm::vec3(1.0f);
    white.useLighting = false;
    graphics->addMaterial("white", white);

    Material purble;
    purble.shaderName = "default";
    purble.color = glm::vec3(0.69f, 0.420f, 0.666f);
    purble.useLighting = false;
    graphics->addMaterial("purble", purble);

    // Initialize the gameworld
    m_gw = std::make_shared<GameWorld>();

    m_gw->registerUISystem(std::make_shared<UIClickSystem>(100));
    m_gw->registerUISystem(std::make_shared<UIDrawer>(200));
    m_gw->registerUISystem(std::make_shared<TextDrawer>(300));

    std::shared_ptr<UIObject> logo = std::make_shared<UIObject>("logo", m_gw->getNewObjID(), CENTER);
    UITransform t = UITransform(CENTER, glm::vec2(0.0f, 30.0f), glm::vec2(504, 162));
    logo->addComponent(std::make_shared<UIRenderable>(logo, t, "uiquad", "title"));
    m_gw->addUIObject(logo);

    std::shared_ptr<UIObject> button = std::make_shared<UIObject>("button", m_gw->getNewObjID(), CENTER);
    t = UITransform(CENTER, glm::vec2(0.0f, -200.0f), glm::vec2(300, 100));
    button->addComponent(std::make_shared<UIHitbox>(button, t, new StartButtonResp(parent)));
    button->addComponent(std::make_shared<UIRenderable>(button, t, "uiquad", "purble"));
    t = UITransform(CENTER, glm::vec2(0.0f, -200.0f));
    button->addComponent(std::make_shared<UIText>(button, t, "Click to begin.", 36, "white"));
    m_gw->addUIObject(button);
}

LandingScreen::~LandingScreen()
{
}

void LandingScreen::tick(float seconds)
{
    m_gw->tick(seconds);
}

void LandingScreen::draw()
{
    m_gw->draw();
}

void LandingScreen::resize(int w, int h)
{
    m_gw->resize(w, h);
}

void LandingScreen::onKeyPressed(QKeyEvent *event) {}
void LandingScreen::onKeyHeld(QKeyEvent *event) {}
void LandingScreen::onKeyReleased(QKeyEvent *event) {}
void LandingScreen::onMousePressed(QMouseEvent *event) {}
void LandingScreen::onMouseReleased(QMouseEvent *event) {}
void LandingScreen::onMouseMoved(QMouseEvent *event, int deltaX, int deltaY) {}
void LandingScreen::onScroll(QWheelEvent *event) {}


void LandingScreen::onFocusGained()
{
}

void LandingScreen::onFocusLost()
{
}
