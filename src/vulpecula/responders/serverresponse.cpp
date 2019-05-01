#include "serverresponse.h"
#include "vulpecula/MainScreen.h"
#include "engine/frame/Application.h"
#include "view.h"

ServerResponse::ServerResponse(Application *app) :
    m_app(app)
{
}

ServerResponse::~ServerResponse() {

}

void ServerResponse::onHover() {

}

void ServerResponse::onPress() {

}

void ServerResponse::onHold() {

}

void ServerResponse::onRelease() {
    //m_app->getViewPtr()->captureMouse(true);
    m_app->popScreen();
    m_app->pushScreen(std::make_shared<MainScreen>(m_app, true));
}
