#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "System.h"
#include "engine/components/CAudioSource.h"
#include "engine/components/CCamera.h"

#include "fmod/fmod.hpp"

#define NUM_CHANNELS 128

class QTemporaryFile;

class AudioSystem : public System
{
public:
    AudioSystem(int priority, std::shared_ptr<GameObject> listener);
    ~AudioSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    void errChk(FMOD_RESULT res);
    void loadSound(std::shared_ptr<CAudioSource> s);

    float m_oldX, m_oldY, m_oldZ;

    FMOD::System *m_sys;
    std::shared_ptr<CCamera> m_listener;

    QSet<std::shared_ptr<CAudioSource>> m_sources;
    QHash<QString, QTemporaryFile *> m_files;
};

#endif // AUDIOSYSTEM_H
