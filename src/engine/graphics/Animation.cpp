#include "Animation.h"

#include <assimp/anim.h>

Animation::Animation() :
    Animation("AnimationNotFound")
{
}

Animation::Animation(QString name) :
    m_name(name),
    m_duration(0.0f),
    m_ticksPerSec(25.0f),
    //m_looping(false)
    m_looping(true) // TODO: this is a placeholder
{
}

Animation::~Animation()
{
}

void Animation::addChannel(QString name, Animation::Channel c)
{
    m_channels.insert(name, Channel(c));
}

void Animation::setDuration(int frames)
{
    m_duration = frames;
}

void Animation::setTicksPerSec(float rate)
{
    m_ticksPerSec = (rate != 0) ? rate : 30.0f;
}

void Animation::setLooping(bool value)
{
    m_looping = value;
}

QString Animation::getName()
{
    return m_name;
}

float Animation::getDuration()
{
    return m_duration;
}

float Animation::getTicksPerSec()
{
    return m_ticksPerSec;
}

bool Animation::shouldLoop()
{
    return m_looping;
}

bool Animation::hasKeys(QString boneName)
{
    return m_channels.contains(boneName);
}

Animation::Channel Animation::getKeys(QString boneName)
{
    return m_channels.value(boneName);
}


