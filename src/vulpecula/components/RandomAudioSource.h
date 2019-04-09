#ifndef RANDOMAUDIOSOURCE_H
#define RANDOMAUDIOSOURCE_H

#include "engine/components/CAudioSource.h"

#include <QStringList>
#include <random>

class RandomAudioSource : public CAudioSource
{
public:
    RandomAudioSource(std::shared_ptr<GameObject> parent, QString path);
    RandomAudioSource(std::shared_ptr<GameObject> parent, QString path, QString channel);
    ~RandomAudioSource();

    void playLooping();

    void addFile(QString path);
    // void addFile(QString path, float weight); // TODO: weighted probabilities
    void removeFile(QString path);

    void setWaitInterval(float min_wait_sec, float max_wait_sec);
    void startNewWait();
    void resetWaitTime();
    bool incrementWait(float seconds);
    bool isWaiting();

    void chooseNewFile();

    //void setPanInterval(float min_pan, float max_pan);
    //void forceTrigger();


private:
    QStringList m_files;

    float m_min_wait, m_max_wait, m_cur_wait, m_wait_target;
    std::mt19937 m_rng_engine;
};

#endif // RANDOMAUDIOSOURCE_H
