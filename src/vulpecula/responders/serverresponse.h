#ifndef SERVERRESPONSE_H
#define SERVERRESPONSE_H

#include "engine/ui/ClickResponse.h"
#include "engine/ui/UIRenderable.h"

class Application;

class ServerResponse : public ClickResponse
{
public:
    ServerResponse(Application *app, UIRenderable *renderable);
    ~ServerResponse();

    void onEnter();
    void onHover();
    void onExit();
    void onPress();
    void onHold();
    void onRelease();

private:
    Application *m_app;
    UIRenderable *m_renderable;
};

#endif // SERVERRESPONSE_H
