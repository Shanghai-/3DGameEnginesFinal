#include "VulpeculaApp.h"

#include "MainScreen.h"
#include "menuscreen.h"
#include "view.h"

VulpeculaApp::VulpeculaApp(View *v) :
    Application(v)
{
    // TODO: Replace this with the menu screen
    //v->captureMouse(true);
    v->captureMouse(false);
    pushScreen(std::make_shared<MenuScreen>(this));
    //pushScreen(std::make_shared<MainScreen>(this));
}

VulpeculaApp::~VulpeculaApp()
{
    clearScreens();
}
