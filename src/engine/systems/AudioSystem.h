#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "System.h"
#include "engine/components/CAudioSource.h"
#include "engine/components/CCamera.h"

#include "fmod/fmod.hpp"

#include <QVector>

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

    void createChannel(const QString &name);
    void deleteChannel(const QString &name);
    FMOD::ChannelGroup *getChannel(const QString &name);
    void setChannelChild(const QString &parent, const QString &child);
    void setChannelVolume(const QString &name, const float &volume);
    void setChannelMuted(const QString &name, const bool &isMuted);
    void fadeChannelVolume(const QString &name, const float &targetVolume, const float &timeInSeconds);

    FMOD::DSP *createEffect(FMOD_DSP_TYPE type);
    void addEffectAtStart(FMOD::DSP *effect, QString effectName, QString channelName, bool pre_fader);
    void addEffectAtEnd(FMOD::DSP *effect, QString effectName, QString channelName, bool pre_fader);
    void addEffectAtIndex(FMOD::DSP *effect, QString effectName, QString channelName, bool pre_fader, int index);
    FMOD::DSP *getEffect(QString effectName, QString channelName);
    void removeEffect(QString effectName, QString channelName);

private:
    struct FadeInfo {
        QString channel = "";
        float startingVol = 0;
        float endingVol = 0;
        float endingTime = 0;
        float curTime = 0;
    };

    void errChk(FMOD_RESULT res);
    void loadSound(std::shared_ptr<CAudioSource> s);

    float m_oldX, m_oldY, m_oldZ;

    FMOD::System *m_sys;
    std::shared_ptr<CCamera> m_listener;

    QSet<std::shared_ptr<CAudioSource>> m_sources;
    QHash<QString, QTemporaryFile *> m_files;
    QHash<QString, FMOD::ChannelGroup *> m_channels;
    QHash<QString, FMOD::DSP *> m_effects;
    QVector<FadeInfo> m_fades;
};

#endif // AUDIOSYSTEM_H
