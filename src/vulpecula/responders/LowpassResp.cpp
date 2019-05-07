#include "LowpassResp.h"

#include <iostream>

LowpassResp::LowpassResp(glm::vec3 pos, AudioSystem *audio) :
    m_pos(pos),
    m_audio(audio)
{
}

LowpassResp::~LowpassResp()
{
}

void LowpassResp::onCollide(std::shared_ptr<GameObject> other)
{
}

void LowpassResp::onCollisionEnd(std::shared_ptr<GameObject> other)
{
    if (other->getName() == "Player") {
        FMOD::DSP *existing = m_audio->getEffect("Lowpass", "Ambient");
        if (other->getComponent<CTransform>()->pos.x > m_pos.x) {
            if (!existing) {
                // add lowpass
                existing = m_audio->createEffect(FMOD_DSP_TYPE_LOWPASS);
                existing->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 600.0f);
                existing->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, 1.2f);

                m_audio->addEffectAtStart(existing, "Lowpass", "Ambient", false);

                FMOD::DSP *other = m_audio->createEffect(FMOD_DSP_TYPE_LOWPASS);
                other->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 950.0f);
                m_audio->addEffectAtStart(other, "Lowpass", "SFX", false);

                FMOD::DSP *third = m_audio->createEffect(FMOD_DSP_TYPE_LOWPASS);
                other->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 950.0f);
                m_audio->addEffectAtStart(third, "Lowpass", "Music", false);
            }
        } else {
            // remove lowpass
            if (existing) {
                m_audio->removeEffect("Lowpass", "Ambient");
                m_audio->removeEffect("Lowpass", "SFX");
                m_audio->removeEffect("Lowpass", "Music");
            }
        }
    }
}
