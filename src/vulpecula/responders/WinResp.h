#ifndef WINRESP_H
#define WINRESP_H

#include "engine/components/responders/CollisionResponse.h"

class WinResp : public CollisionResponse
{
public:
    WinResp(GameWorld *gw);
    ~WinResp();

    void onCollide(std::shared_ptr<GameObject> other);

private:
    GameWorld *m_gw;
};

#endif // WINRESP_H
