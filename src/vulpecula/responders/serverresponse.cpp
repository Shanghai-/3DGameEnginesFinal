#include "serverresponse.h"
#include "vulpecula/MainScreen.h"
#include "engine/frame/Application.h"
#include "view.h"

ServerResponse::ServerResponse(Application *app, UIRenderable *renderable) :
    m_app(app),
    m_renderable(renderable)
{
}

ServerResponse::~ServerResponse() {

}

void ServerResponse::onEnter()
{
    Graphics *g = Graphics::getGlobalInstance();

    std::string mat = m_renderable->getMaterial();
    Material m = g->getMaterial(mat);
    m.textureName = "ServerButtonHover";
    g->addMaterial(mat, m);
}

void ServerResponse::onHover() {
}

void ServerResponse::onExit()
{
    Graphics *g = Graphics::getGlobalInstance();

    std::string mat = m_renderable->getMaterial();
    Material m = g->getMaterial(mat);
    m.textureName = "ServerButton";
    g->addMaterial(mat, m);
}

void ServerResponse::onPress() {
}

void ServerResponse::onHold() {

}

void ServerResponse::onRelease() {
    m_app->popScreen();
    m_app->pushScreen(std::make_shared<MainScreen>(m_app, true));
}
