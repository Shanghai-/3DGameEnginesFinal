#ifndef WINRESP_H
#define WINRESP_H

#include "engine/components/responders/CollisionResponse.h"

class Application;
class AudioSystem;

class WinResp : public CollisionResponse
{
public:
    WinResp(GameWorld *gw, Application *app, AudioSystem *audio);
    ~WinResp();

    void onCollide(std::shared_ptr<GameObject> other);

private:
    GameWorld *m_gw;
    Application *m_app;
    AudioSystem *m_audio;
};

#endif // WINRESP_H
