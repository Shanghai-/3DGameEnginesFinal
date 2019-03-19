#include "AudioSystem.h"

#include "engine/components/CAudioSource.h"
#include "fmod/fmod_errors.h"

#include <iostream>

AudioSystem::AudioSystem(int priority) :
    System(priority)
{
    FMOD_RESULT res = FMOD::System_Create(&m_sys);
    if (res != FMOD_OK)
    {
        std::cerr << "FMOD create error! (" << res << "): " << FMOD_ErrorString(res) << std::endl;
    } else {
        res = m_sys->init(512, FMOD_INIT_NORMAL, nullptr);
        if (res != FMOD_OK) {
            std::cerr << "FMOD init error! (" << res << "): " << FMOD_ErrorString(res) << std::endl;
        }
    }
}

AudioSystem::~AudioSystem()
{
}

QString AudioSystem::getComponentType() const
{
    return typeid(CAudioSource).name();
}

void AudioSystem::addComponent(const std::shared_ptr<Component> &c)
{
}

void AudioSystem::removeComponent(const std::shared_ptr<Component> &c)
{
}

void AudioSystem::tick(float seconds)
{
}
