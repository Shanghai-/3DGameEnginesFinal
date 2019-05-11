#include "clientresponse.h"
#include "vulpecula/MainScreen.h"
#include "engine/frame/Application.h"
#include "engine/graphics/Graphics.h"

ClientResponse::ClientResponse(Application *app, UIRenderable *renderable) :
    m_app(app),
    m_renderable(renderable)
{
}


ClientResponse::~ClientResponse() {

}

void ClientResponse::onEnter()
{
    Graphics *g = Graphics::getGlobalInstance();

    std::string mat = m_renderable->getMaterial();
    Material m = g->getMaterial(mat);
    m.textureName = "ClientButtonHover";
    g->addMaterial(mat, m);
}

void ClientResponse::onHover() {
}

void ClientResponse::onExit()
{
    Graphics *g = Graphics::getGlobalInstance();

    std::string mat = m_renderable->getMaterial();
    Material m = g->getMaterial(mat);
    m.textureName = "ClientButton";
    g->addMaterial(mat, m);
}

void ClientResponse::onPress() {

}

void ClientResponse::onHold() {

}

void ClientResponse::onRelease() {
    m_app->popScreen();
    m_app->pushScreen(std::make_shared<MainScreen>(m_app, false));
}
