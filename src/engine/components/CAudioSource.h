#ifndef CAUDIOSOURCE_H
#define CAUDIOSOURCE_H

#include "Component.h"

#include "fmod.hpp"

class CAudioSource : public Component
{
public:
    CAudioSource(std::shared_ptr<GameObject> parent, QString path, bool playOnAwake);
    CAudioSource(std::shared_ptr<GameObject> parent, QString path, bool playOnAwake, int channel);
    ~CAudioSource();

    void setFile(QString path);
    bool fileChanged() { return m_path_changed; }
    void resetFile() { m_path_changed = false; }

    void setMuted(bool val) { m_muted = val; }
    void setAmbient(bool val) { m_ambient = val; }
    void setStereoSpread(float val) { m_stereo_spread = val; }

    void play();
    void playOnce();
    void playLooping();
    void pause();
    void stop();

    QString getPath() { return m_path; }
    float getStereoSpread() { return m_stereo_spread; }
    int getChannelGroup() { return m_channel_grp; }

    bool isAmbient() { return m_ambient; }
    bool isPlaying() { return m_playing && !m_stopped; }
    bool isPaused() { return !m_playing && !m_stopped; }
    bool isStopped() { return !m_playing && m_stopped; }
    bool shouldLoop() { return m_looping; }
    bool isMuted() { return m_muted; }

    FMOD::Sound *clip = nullptr;
    FMOD::Channel *channel = nullptr;

private:
    QString m_path;
    bool m_path_changed;

    bool m_ambient;
    bool m_playing;
    bool m_looping;
    bool m_stopped;
    bool m_muted;

    float m_stereo_spread;

    int m_channel_grp;
};

#endif // CAUDIOSOURCE_H
