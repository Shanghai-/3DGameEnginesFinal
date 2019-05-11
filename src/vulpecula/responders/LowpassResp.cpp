#include "LowpassResp.h"

LowpassResp::LowpassResp(glm::vec3 pos, AudioSystem *audio, ParticleSys *ps) :
    m_pos(pos),
    m_audio(audio),
    m_particles(ps)
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
                // Add lowpass to ambient channel at 600 Hz
                existing = m_audio->createEffect(FMOD_DSP_TYPE_LOWPASS);
                existing->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 600.0f);
                existing->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, 1.2f);
                m_audio->addEffectAtStart(existing, "Lowpass", "Ambient", false);

                // Add lowpass to SFX (incidental sounds) at 1.5k Hz
                FMOD::DSP *other = m_audio->createEffect(FMOD_DSP_TYPE_LOWPASS);
                other->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 1500.0f);
                other->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, 1.3f);
                m_audio->addEffectAtStart(other, "Lowpass", "SFX", false);

                // Add lowpass to music at 880 Hz
                FMOD::DSP *third = m_audio->createEffect(FMOD_DSP_TYPE_LOWPASS);
                third->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 880.0f);
                third->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, 1.3f);
                m_audio->addEffectAtStart(third, "Lowpass", "Music", false);

                m_particles->setHidden(true);
            }
        } else {
            // remove lowpass
            if (existing) {
                m_audio->removeEffect("Lowpass", "Ambient");
                m_audio->removeEffect("Lowpass", "SFX");
                m_audio->removeEffect("Lowpass", "Music");

                m_particles->setHidden(false);
            }
        }
    }
}
