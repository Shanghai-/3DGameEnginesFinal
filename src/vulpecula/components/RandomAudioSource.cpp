#include "RandomAudioSource.h"

RandomAudioSource::RandomAudioSource(std::shared_ptr<GameObject> parent, QString path) :
    RandomAudioSource(parent, path, "Master")

{
}

RandomAudioSource::RandomAudioSource(std::shared_ptr<GameObject> parent, QString path, QString channel) :
    CAudioSource(parent, path, false, channel),
    m_min_wait(1.50f),
    m_max_wait(5.0f),
    m_cur_wait(-1.0f),
    m_wait_target(5.0f)
{
    std::random_device r;
    m_rng_engine.seed(r());
    m_files.append(path);
}

RandomAudioSource::~RandomAudioSource()
{
}

void RandomAudioSource::playLooping()
{
    // We don't want the clips to loop, so we override playLooping to act like a normal play
    playOnce();
}

void RandomAudioSource::addFile(QString path)
{
    m_files.append(path);
}

void RandomAudioSource::removeFile(QString path)
{
    m_files.removeOne(path);
}

void RandomAudioSource::setWaitInterval(float min_wait_sec, float max_wait_sec)
{
    m_min_wait = min_wait_sec;
    m_max_wait = max_wait_sec;
    m_cur_wait = 0.0f;
}

void RandomAudioSource::startNewWait()
{
    std::uniform_real_distribution<float> dist(m_min_wait, m_max_wait);
    m_wait_target = dist(m_rng_engine);
    m_cur_wait = 0.0f;
}

void RandomAudioSource::resetWaitTime()
{
    m_cur_wait = 0.0f;
}

bool RandomAudioSource::incrementWait(float seconds)
{
    m_cur_wait += seconds;

    if (m_cur_wait >= m_wait_target) {
        m_cur_wait = -1.0f;
        return true;
    }

    return false;
}

bool RandomAudioSource::isWaiting()
{
    return m_cur_wait > -0.5f;
}


void RandomAudioSource::chooseNewFile()
{
    std::uniform_int_distribution<int> dist(0, m_files.size() - 1);
    int index = dist(m_rng_engine);
    setFile(m_files.at(index));
}

