#include "AABB.h"

#include "Graphics.h"

AABB::AABB() :
    m_min(glm::vec4(glm::vec3(std::numeric_limits<float>::infinity()), 1.0f)),
    m_max(glm::vec4(glm::vec3(-std::numeric_limits<float>::infinity()), 1.0f))
{
    m_points.reserve(8);
}

AABB::AABB(glm::vec3 min, glm::vec3 max) :
    m_min(glm::vec4(min, 1.0f)),
    m_max(glm::vec4(max, 1.0f))
{
    m_points.reserve(8);
    calculatePoints();
}

AABB::~AABB()
{
}

void AABB::encapsulate(glm::vec3 point)
{
    encapsulate(point.x, point.y, point.z);
}

void AABB::encapsulate(float x, float y, float z)
{
    //std::cout << m_min.x << ", " << m_min.y << ", " << m_min.z << std::endl;
    //std::cout << m_max.x << ", " << m_max.y << ", " << m_max.z << std::endl;
    if (x < m_min.x) m_min.x = x;
    if (y < m_min.y) m_min.y = y;
    if (z < m_min.z) m_min.z = z;
    if (x > m_max.x) m_max.x = x;
    if (y > m_max.y) m_max.y = y;
    if (z > m_max.z) m_max.z = z;
}

QVector<glm::vec4> *AABB::getPoints()
{
    return &m_points;
}

void AABB::updateBounds(glm::mat4 transform)
{
    float min_x, min_y, min_z, max_x, max_y, max_z;
    min_x = min_y = min_z = std::numeric_limits<float>::infinity();
    max_x = max_y = max_z = -std::numeric_limits<float>::infinity();

    for (int i = 0; i < m_points.length(); i++) {
        glm::vec4 translated = transform * m_points[i];
        if (translated.x < min_x) min_x = translated.x;
        if (translated.y < min_y) min_y = translated.y;
        if (translated.z < min_z) min_z = translated.z;
        if (translated.x > max_x) max_x = translated.x;
        if (translated.y > max_y) max_y = translated.y;
        if (translated.z > max_z) max_z = translated.z;
    }

    m_min = glm::vec4(min_x, min_y, min_z, 1.0f);
    m_max = glm::vec4(max_x, max_y, max_z, 1.0f);
    calculatePoints();
}

void AABB::calculatePoints()
{
    m_points.clear();
    m_points.append(m_min);
    m_points.append(m_max);
    m_points.append(glm::vec4(m_min.x, m_min.y, m_max.z, 1.0f));
    m_points.append(glm::vec4(m_min.x, m_max.y, m_min.z, 1.0f));
    m_points.append(glm::vec4(m_max.x, m_min.y, m_min.z, 1.0f));
    m_points.append(glm::vec4(m_max.x, m_max.y, m_min.z, 1.0f));
    m_points.append(glm::vec4(m_max.x, m_min.y, m_max.z, 1.0f));
    m_points.append(glm::vec4(m_min.x, m_max.y, m_max.z, 1.0f));
}

glm::vec4 AABB::getMin() { return m_min; }
glm::vec4 AABB::getMax() { return m_max; }

float AABB::getXSize() { return m_max.x - m_min.x; }
float AABB::getYSize() { return m_max.y - m_min.y; }
float AABB::getZSize() { return m_max.z - m_min.z; }

void AABB::draw(Graphics *g)
{
    glm::vec4 intermediary = m_max - m_min;
    glm::vec3 scale(intermediary.x, intermediary.y, intermediary.z);
    g->scale(scale);
    g->drawShape("wire_cube");
}
