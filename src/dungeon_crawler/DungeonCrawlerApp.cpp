#include "DungeonCrawlerApp.h"

#include "LandingScreen.h"
#include "view.h"

DungeonCrawlerApp::DungeonCrawlerApp(View *v) :
    Application(v)
{
    v->captureMouse(false);
    pushScreen(std::make_shared<LandingScreen>(this));
}

DungeonCrawlerApp::~DungeonCrawlerApp()
{
    clearScreens();
}
