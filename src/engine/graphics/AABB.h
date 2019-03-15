#ifndef AABB_H
#define AABB_H

#include "engine/util/CommonIncludes.h"

#include <QVector>

class Graphics;

class AABB
{
public:
    AABB();
    AABB(glm::vec3 min, glm::vec3 max);
    ~AABB();

    void encapsulate(glm::vec3 point);
    void encapsulate(float x, float y, float z);

    QVector<glm::vec4> *getPoints();
    void updateBounds(glm::mat4 transform);

    glm::vec4 getMin();
    glm::vec4 getMax();

    float getXSize();
    float getYSize();
    float getZSize();

    void draw(Graphics *g);

private:
    void calculatePoints();

    QVector<glm::vec4> m_points;
    glm::vec4 m_min, m_max;
};

#endif // AABB_H
