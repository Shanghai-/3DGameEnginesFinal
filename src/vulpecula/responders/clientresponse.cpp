#include "clientresponse.h"
#include "vulpecula/MainScreen.h"
#include "engine/frame/Application.h"
#include "view.h"

ClientResponse::ClientResponse(Application *app) :
    m_app(app)
{
}


ClientResponse::~ClientResponse() {

}

void ClientResponse::onHover() {

}

void ClientResponse::onPress() {

}

void ClientResponse::onHold() {

}

void ClientResponse::onRelease() {
    m_app->getViewPtr()->captureMouse(true);
    m_app->popScreen();
    m_app->pushScreen(std::make_shared<MainScreen>(m_app, false));
}
