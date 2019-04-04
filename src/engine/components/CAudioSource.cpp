#include "CAudioSource.h"

CAudioSource::CAudioSource(std::shared_ptr<GameObject> parent, QString path, bool playOnAwake) :
    CAudioSource(parent, path, playOnAwake, 0)
{
}

CAudioSource::CAudioSource(std::shared_ptr<GameObject> parent, QString path, bool playOnAwake, int channel) :
    Component(parent),
    m_path(path),
    m_path_changed(false),
    m_ambient(false),
    m_playing(playOnAwake),
    m_looping(false),
    m_stopped(false),
    m_muted(false),
    m_stereo_spread(0.0f),
    m_channel_grp(channel)
{
}

CAudioSource::~CAudioSource()
{
}

void CAudioSource::setFile(QString path)
{
    m_path = path;
    m_path_changed = true;
}

void CAudioSource::play()
{
    m_playing = true;
    m_stopped = false;
}

void CAudioSource::playOnce()
{
    m_playing = true;
    m_stopped = false;
    m_looping = false;
}

void CAudioSource::playLooping()
{
    m_playing = true;
    m_stopped = false;
    m_looping = true;
}

void CAudioSource::pause()
{
    m_playing = false;
    m_stopped = false;
}

void CAudioSource::stop()
{
    m_playing = false;
    m_stopped = true;
}
