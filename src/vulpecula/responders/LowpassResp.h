#ifndef LOWPASSRESP_H
#define LOWPASSRESP_H

#include "engine/components/responders/CollisionResponse.h"
#include "engine/components/CTransform.h"
#include "engine/systems/AudioSystem.h"

class LowpassResp : public CollisionResponse
{
public:
    LowpassResp(glm::vec3 pos, AudioSystem *audio);
    ~LowpassResp();

    void onCollide(std::shared_ptr<GameObject> other);
    void onCollisionEnd(std::shared_ptr<GameObject> other);

private:
    glm::vec3 m_pos;
    AudioSystem *m_audio;
};

#endif // LOWPASSRESP_H
