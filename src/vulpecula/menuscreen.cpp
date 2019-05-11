#include "menuscreen.h"

#include "engine/objectManagement/GameWorld.h"
#include "engine/graphics/Graphics.h"

#include "engine/ui/UIDrawer.h"
#include "engine/ui/TextDrawer.h"
#include "engine/ui/UIClickSystem.h"
#include "engine/ui/UIObject.h"
#include "engine/ui/UIHitbox.h"
#include "engine/ui/UIRenderable.h"
#include "engine/ui/UIText.h"
#include "vulpecula/responders/serverresponse.h"
#include "vulpecula/responders/clientresponse.h"

MenuScreen::MenuScreen(Application *parent) : m_parent(parent)
{
    std::string basePath = "/course/cs1950u/.archive/2019/student/vulpecula/ui/";

    // Set up graphics
    Graphics *graphics = Graphics::getGlobalInstance();
    graphics->addFont("LatoThin", "/course/cs1950u/.archive/2019/student/vulpecula/Lato-Thin.ttf");

    // Pure white (for UI)
    Material white;
    white.color = glm::vec3(1.0f);
    white.useLighting = false;
    graphics->addMaterial("UIWhite", white);

    graphics->addTexture("MenuFox", basePath + "menu_fox.png");
    Material m;
    m.useLighting = false;
    m.textureName = "MenuFox";
    graphics->addMaterial("MenuFox", m);

    graphics->addTexture("MenuTitle", basePath + "menu_title.png");
    m.textureName = "MenuTitle";
    graphics->addMaterial("MenuTitle", m);

    graphics->addTexture("ClientButton", basePath + "client_button.png");
    graphics->addTexture("ClientButtonHover", basePath + "client_button_hover.png");
    m.textureName = "ClientButton";
    graphics->addMaterial("ClientButton", m);

    graphics->addTexture("ServerButton", basePath + "server_button.png");
    graphics->addTexture("ServerButtonHover", basePath + "server_button_hover.png");
    m.textureName = "ServerButton";
    graphics->addMaterial("ServerButton", m);

    // Initialize the gameworld
    m_gw = std::make_shared<GameWorld>();

    m_gw->registerUISystem(std::make_shared<UIClickSystem>(100));
    m_gw->registerUISystem(std::make_shared<UIDrawer>(200));
    m_gw->registerUISystem(std::make_shared<TextDrawer>(300));

    // Nice fox art
    std::shared_ptr<UIObject> titleArt = std::make_shared<UIObject>("FoxArt", m_gw->getNewObjID(), TOP_CENTER);
    UITransform t = UITransform(CENTER, glm::vec2(0, -240), glm::vec2(522, 337));
    titleArt->addComponent(std::make_shared<UIRenderable>(titleArt, t, "uiquad", "MenuFox"));
    m_gw->addUIObject(titleArt);

    // Game Title
    std::shared_ptr<UIObject> title = std::make_shared<UIObject>("Title", m_gw->getNewObjID(), CENTER);
    t = UITransform(BOT_CENTER, glm::vec2(0, -54), glm::vec2(1044, 224));
    title->addComponent(std::make_shared<UIRenderable>(title, t, "uiquad", "MenuTitle"));
    m_gw->addUIObject(title);

    // Client play button
    std::shared_ptr<UIObject> clientButton = std::make_shared<UIObject>("ClientButton", m_gw->getNewObjID(), BOT_CENTER);
    t = UITransform(CENTER, glm::vec2(0, 370), glm::vec2(426, 165));
    auto renderable = std::make_shared<UIRenderable>(clientButton, t, "uiquad", "ClientButton");
    clientButton->addComponent(std::make_shared<UIHitbox>(clientButton, t, new ClientResponse(parent, renderable.get())));
    clientButton->addComponent(renderable);
    m_gw->addUIObject(clientButton);

    // Server play button
    std::shared_ptr<UIObject> serverButton = std::make_shared<UIObject>("ServerButton", m_gw->getNewObjID(), BOT_CENTER);
    t = UITransform(CENTER, glm::vec2(0, 150), glm::vec2(470, 170));
    renderable = std::make_shared<UIRenderable>(serverButton, t, "uiquad", "ServerButton");
    serverButton->addComponent(std::make_shared<UIHitbox>(serverButton, t, new ServerResponse(parent, renderable.get())));
    serverButton->addComponent(renderable);
    m_gw->addUIObject(serverButton);

    // Hot tip telling people how to make the game stop
    std::shared_ptr<UIObject> quitText = std::make_shared<UIObject>("QuitText", m_gw->getNewObjID(), TOP_LEFT);
    t = UITransform(TOP_LEFT, glm::vec2(18, -18));
    quitText->addComponent(std::make_shared<UIText>(quitText, t, "Press [Esc] at any time to quit.", 38.0f, "UIWhite", "LatoThin"));
    m_gw->addUIObject(quitText);
}

MenuScreen::~MenuScreen() {

}

void MenuScreen::tick(float seconds)
{
    m_gw->tick(seconds);
}

void MenuScreen::draw()
{
    m_gw->draw();
}

void MenuScreen::resize(int w, int h)
{
    m_gw->resize(w, h);
}

void MenuScreen::onFocusGained()
{

}

void MenuScreen::onFocusLost()
{
    Graphics *graphics = Graphics::getGlobalInstance();
    graphics->removeTexture("MenuFox");
    graphics->removeTexture("MenuTitle");
    graphics->removeTexture("ClientButton");
    graphics->removeTexture("ClientButtonHover");
    graphics->removeTexture("ServerButton");
    graphics->removeTexture("ServerButtonHover");

    graphics->removeMaterial("MenuFox");
    graphics->removeMaterial("MenuTitle");
    graphics->removeMaterial("ClientButton");
    graphics->removeMaterial("ServerButton");
}

void MenuScreen::onKeyPressed(QKeyEvent *event) { }
void MenuScreen::onKeyHeld(QKeyEvent *event) { }
void MenuScreen::onKeyReleased(QKeyEvent *event) { }
void MenuScreen::onMousePressed(QMouseEvent *event) { }
void MenuScreen::onMouseReleased(QMouseEvent *event) { }
void MenuScreen::onMouseMoved(QMouseEvent *event, int deltaX, int deltaY) { }
void MenuScreen::onScroll(QWheelEvent *event) { }
