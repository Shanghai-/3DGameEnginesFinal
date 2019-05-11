#ifndef LOWPASSRESP_H
#define LOWPASSRESP_H

#include "engine/components/responders/CollisionResponse.h"
#include "engine/components/CTransform.h"
#include "engine/systems/AudioSystem.h"

#include "vulpecula/systems/particlesys.h"

class LowpassResp : public CollisionResponse
{
public:
    LowpassResp(glm::vec3 pos, AudioSystem *audio, ParticleSys *ps);
    ~LowpassResp();

    void onCollide(std::shared_ptr<GameObject> other);
    void onCollisionEnd(std::shared_ptr<GameObject> other);

private:
    glm::vec3 m_pos;
    AudioSystem *m_audio;
    ParticleSys *m_particles;
};

#endif // LOWPASSRESP_H
