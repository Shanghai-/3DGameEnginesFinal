#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "System.h"

#include "fmod/fmod.hpp"

class AudioSystem : public System
{
public:
    AudioSystem(int priority);
    ~AudioSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    FMOD::System *m_sys;
};

#endif // AUDIOSYSTEM_H
