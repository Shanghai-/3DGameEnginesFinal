#ifndef CAUDIOSOURCE_H
#define CAUDIOSOURCE_H

#include "Component.h"

#include "fmod.hpp"

class CAudioSource : public Component
{
public:
    CAudioSource(std::shared_ptr<GameObject> parent, QString path);
    CAudioSource(std::shared_ptr<GameObject> parent, QString path, bool follows_head);
    ~CAudioSource();

    QString getPath() { return m_path; }
    bool isAmbient() { return m_ambient; }

    FMOD::Sound *clip = nullptr;
    FMOD::Channel *channel = nullptr;

private:
    QString m_path;
    bool m_ambient;
};

#endif // CAUDIOSOURCE_H
