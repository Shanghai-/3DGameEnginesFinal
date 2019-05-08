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
    // Set up graphics
    Graphics *graphics = Graphics::getGlobalInstance();

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

//    m_gw->registerUISystem(std::make_shared<UIClickSystem>(100));
//    m_gw->registerUISystem(std::make_shared<UIDrawer>(200));
//    m_gw->registerUISystem(std::make_shared<TextDrawer>(300));

//    std::shared_ptr<UIObject> serverButton = std::make_shared<UIObject>("button", m_gw->getNewObjID(), CENTER);
//    UITransform t = UITransform(CENTER, glm::vec2(200.f, 0.0f), glm::vec2(300, 100));
//    serverButton->addComponent(std::make_shared<UIHitbox>(serverButton, t, new ServerResponse(parent)));
//    serverButton->addComponent(std::make_shared<UIRenderable>(serverButton, t, "uiquad", "purble"));
//    t = UITransform(CENTER, glm::vec2(-200.f, 0.0f));
//    serverButton->addComponent(std::make_shared<UIText>(serverButton, t, "Server", 36, "white"));
//    m_gw->addUIObject(serverButton);

//    std::shared_ptr<UIObject> clientButton = std::make_shared<UIObject>("button", m_gw->getNewObjID(), CENTER);
//    t = UITransform(CENTER, glm::vec2(-200.f, 0.0f), glm::vec2(300, 100));
//    clientButton->addComponent(std::make_shared<UIHitbox>(clientButton, t, new ClientResponse(parent)));
//    clientButton->addComponent(std::make_shared<UIRenderable>(clientButton, t, "uiquad", "purble"));
//    t = UITransform(CENTER, glm::vec2(200.f, 0.0f));
//    clientButton->addComponent(std::make_shared<UIText>(clientButton, t, "Client", 36, "white"));
//    m_gw->addUIObject(clientButton);


    m_particle = true;

    m_partSys = std::make_shared<ParticleSys>(100);
    m_gw->registerForDraw(m_partSys);
    //m_gw->registerForTick(partSys);

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

}

void MenuScreen::onKeyPressed(QKeyEvent *event)
{
    if(event->key() == Qt::Key_P) {
        if(m_particle) {
            m_gw->removeFromDraw(m_partSys);
        }
        else {
            m_partSys = std::make_shared<ParticleSys>(100);
            m_gw->registerForDraw(m_partSys);
        }
        m_particle = !m_particle;
    }
}

void MenuScreen::onKeyHeld(QKeyEvent *event)
{

}

void MenuScreen::onKeyReleased(QKeyEvent *event)
{

}

void MenuScreen::onMousePressed(QMouseEvent *event)
{

}

void MenuScreen::onMouseReleased(QMouseEvent *event)
{

}

void MenuScreen::onMouseMoved(QMouseEvent *event, int deltaX, int deltaY)
{

}

void MenuScreen::onScroll(QWheelEvent *event)
{

}
