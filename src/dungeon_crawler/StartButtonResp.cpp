#include "StartButtonResp.h"

#include "DungeonCrawlerApp.h"
#include "GameplayScreen.h"
#include "view.h"

#include <memory>
#include <iostream>

StartButtonResp::StartButtonResp(Application *app) :
    m_app(app)
{
}

StartButtonResp::~StartButtonResp()
{
}

void StartButtonResp::onEnter()
{
}

void StartButtonResp::onHover()
{
}

void StartButtonResp::onExit()
{
}

void StartButtonResp::onPress()
{
}

void StartButtonResp::onHold()
{
}

void StartButtonResp::onRelease()
{
    m_app->popScreen();
    m_app->pushScreen(std::make_shared<GameplayScreen>(m_app));
}
