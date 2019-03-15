#include "WarmupApp.h"
#include "view.h"

WarmupApp::WarmupApp(View *v) :
    Application(v)
{
    v->captureMouse(true);
    pushScreen(std::make_shared<MainScreen>(this));
}

WarmupApp::~WarmupApp()
{
    clearScreens();
}
