#ifndef SERVERRESPONSE_H
#define SERVERRESPONSE_H

#include "engine/ui/ClickResponse.h"


class Application;

class ServerResponse : public ClickResponse
{
public:
    ServerResponse(Application *app);
    ~ServerResponse();

    void onHover();
    void onPress();
    void onHold();
    void onRelease();
private:
    Application *m_app;

};

#endif // SERVERRESPONSE_H
