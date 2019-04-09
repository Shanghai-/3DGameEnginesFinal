#ifndef RANDOMAUDIOMANAGER_H
#define RANDOMAUDIOMANAGER_H

#include "engine/systems/System.h"
#include "engine/systems/AudioSystem.h"

#include "vulpecula/components/RandomAudioSource.h"

class RandomAudioManager : public System
{
public:
    RandomAudioManager(int priority, const std::shared_ptr<AudioSystem> &as);
    ~RandomAudioManager();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    std::shared_ptr<AudioSystem> m_as;
    QSet<std::shared_ptr<RandomAudioSource>> m_sources;
};

#endif // RANDOMAUDIOMANAGER_H
