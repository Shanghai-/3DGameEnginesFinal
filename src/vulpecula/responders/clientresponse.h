#ifndef CLIENTRESPONSE_H
#define CLIENTRESPONSE_H

#include "engine/ui/ClickResponse.h"

class Application;
class ClientResponse : public ClickResponse
{
public:
    ClientResponse(Application *app);
    ~ClientResponse();

    void onHover();
    void onPress();
    void onHold();
    void onRelease();

    Application *m_app;
};

#endif // CLIENTRESPONSE_H
