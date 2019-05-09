#include "VulpeculaApp.h"

#include "menuscreen.h"
#include "view.h"

VulpeculaApp::VulpeculaApp(View *v) :
    Application(v)
{
    v->captureMouse(false);
    pushScreen(std::make_shared<MenuScreen>(this));
}

VulpeculaApp::~VulpeculaApp()
{
    clearScreens();
}
