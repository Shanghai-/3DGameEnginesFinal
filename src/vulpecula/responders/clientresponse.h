#ifndef CLIENTRESPONSE_H
#define CLIENTRESPONSE_H

#include "engine/ui/ClickResponse.h"
#include "engine/ui/UIRenderable.h"

class Application;

class ClientResponse : public ClickResponse
{
public:
    ClientResponse(Application *app, UIRenderable *renderable);
    ~ClientResponse();

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

#endif // CLIENTRESPONSE_H
