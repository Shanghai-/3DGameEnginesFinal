#ifndef PROGRESSTRACKER_H
#define PROGRESSTRACKER_H

#include "engine/systems/AudioSystem.h"

class ProgressTracker : public System
{
public:
    ProgressTracker(int priority, AudioSystem *audioSys);
    ~ProgressTracker();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    AudioSystem *m_audioSys;

    int m_totalStars;
    int m_curStars;
};

#endif // PROGRESSTRACKER_H
