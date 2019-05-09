#ifndef PROGRESSTRACKER_H
#define PROGRESSTRACKER_H

#include "engine/systems/AudioSystem.h"
#include "vulpecula/scripts/StarCountScript.h"

class Application;

class ProgressTracker : public System
{
public:
    ProgressTracker(int priority, AudioSystem *audioSys,
                    Application *app,
                    StarCountScript *starCounter);
    ~ProgressTracker();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);

private:
    AudioSystem *m_audioSys;
    Application *m_app;
    StarCountScript *m_starCounter;

    int m_totalStars;
    int m_curStars;
};

#endif // PROGRESSTRACKER_H
