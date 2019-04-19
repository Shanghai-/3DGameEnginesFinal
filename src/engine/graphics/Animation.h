#ifndef ANIMATION_H
#define ANIMATION_H

#include <QHash>
#include <QString>
#include <vector>
#include "engine/util/CommonIncludes.h"
#include <glm/gtx/quaternion.hpp>

class Animation
{
public:
    struct Channel {
        std::vector<glm::vec3> positions;
        std::vector<float> pos_frames;

        std::vector<glm::quat> rotations;
        std::vector<float> rot_frames;

        std::vector<glm::vec3> scales;
        std::vector<float> scale_frames;
    };

    Animation();
    Animation(QString name);
    ~Animation();

    void addChannel(QString name, Channel c);
    void setDuration(int frames);
    void setTicksPerSec(float rate);
    void setLooping(bool value);

    QString getName();
    float getDuration();
    float getTicksPerSec();
    bool shouldLoop();

    bool hasKeys(QString boneName);
    Channel getKeys(QString boneName);

private:
    QString m_name;
    QHash<QString, Channel> m_channels;

    float m_duration; // expressed in ticks
    float m_ticksPerSec;

    bool m_looping;
};

#endif // ANIMATION_H
