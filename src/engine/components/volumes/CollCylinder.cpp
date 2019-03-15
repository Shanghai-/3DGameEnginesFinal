#include "CollCylinder.h"

#include "engine/components/CCollider.h"
#include "engine/graphics/Graphics.h"
#include "glm/gtx/norm.hpp"

#include <iostream>

CollCylinder::CollCylinder(glm::vec3 localPos, float height, float radius) :
    CollisionVolume(localPos),
    m_height(height),
    m_radius(radius)
{
    m_r2 = m_radius * m_radius;
}

CollCylinder::~CollCylinder()
{
}

bool CollCylinder::collideBox(CollBox *other)
{
    auto thisTrans = m_owner->getSibling<CTransform>();
    auto otherTrans = other->getOwner()->getSibling<CTransform>();

    glm::vec3 box_dims = other->getDimensions();
    glm::vec3 box_min = otherTrans->toWorld(other->getLocalPos() + (box_dims / -2.0f));
    glm::vec3 box_max = otherTrans->toWorld(other->getLocalPos() + (box_dims / 2.0f));
    glm::vec3 circle_center = thisTrans->toWorld(m_localPos);
    circle_center.y = 0.0f;
    glm::vec3 closest = glm::vec3(glm::clamp(circle_center.x, box_min.x, box_max.x), 0.0f, glm::clamp(circle_center.z, box_min.z, box_max.z));
    float len_squared = glm::length2(closest - circle_center);
    bool circ_coll = len_squared <= m_r2;

    // Check for overlap in the lines
    bool line_coll = (thisTrans->pos.y < otherTrans->pos.y + box_dims.y / 2.0f)
            && (otherTrans->pos.y - box_dims.y / 2.0f < thisTrans->pos.y + m_height);

    if (line_coll && circ_coll) {
        // Skip MTV calculation for trigger intersections
        if (getOwner()->isTrigger() || other->getOwner()->isTrigger()) {
            return true;
        }

        // Calculate circle MTV
        float len = glm::sqrt(len_squared);
        glm::vec3 circ_mtv = ((closest - circle_center) / len) * (m_radius - len);

        // Calculate line MTV
        float aRight = (otherTrans->pos.y + box_dims.y / 2) - thisTrans->pos.y;
        float aLeft = (thisTrans->pos.y + m_height) - (otherTrans->pos.y - (box_dims.y / 2));
        glm::vec3 line_mtv = (aRight < aLeft) ? glm::vec3(0.0f, -aRight, 0.0f) : glm::vec3(0.0f, aLeft, 0.0f);

        // Choose smaller MTV to translate by
        glm::vec3 mtv = (glm::length2(circ_mtv) <= glm::length2(line_mtv)) ? circ_mtv : line_mtv;

        if (thisTrans->is_static()) {
            // If one of the colliders is static, we apply the full MTV to the other
            otherTrans->translate(mtv);
        } else if (otherTrans->is_static()) {
            thisTrans->translate(-mtv);
        } else {
            // Otherwise we split the MTV in half and apply to both
            glm::vec3 half_mtv = mtv / 2.0f;
            otherTrans->translate(half_mtv);
            thisTrans->translate(-half_mtv);
        }

        return true;
    }
    return false;
}

bool CollCylinder::collideSphere(CollSphere *other)
{
    return false;
}

bool CollCylinder::collideCylinder(CollCylinder *other)
{
    auto thisTrans = m_owner->getSibling<CTransform>();
    auto otherTrans = other->getOwner()->getSibling<CTransform>();

    glm::vec3 thisPos = thisTrans->toWorld(getLocalPos());
    glm::vec3 otherPos = otherTrans->toWorld(other->getLocalPos());

    // Remove the y component for circle math
    glm::vec3 thisPos_xz = glm::vec3(thisPos.x, 0.0f, thisPos.z);
    glm::vec3 otherPos_xz = glm::vec3(otherPos.x, 0.0f, otherPos.z);

    // Check for overlap in the circles
    // Use squared length for efficiency
    float len_squared = glm::length2(thisPos_xz - otherPos_xz);
    float radius_sum = m_radius + other->getRadius();
    float coll_thresh = radius_sum * radius_sum;
    bool circ_coll = len_squared <= coll_thresh;

    // Check for overlap in the lines
    bool line_coll = (thisPos.y < otherPos.y + other->getHeight())
            && (otherPos.y < thisPos.y + m_height);

    // If we overlap in both dimensions, that's a collision
    if (line_coll && circ_coll) {

        // Skip MTV calculation for trigger intersections
        if (getOwner()->isTrigger() || other->getOwner()->isTrigger()) {
            return true;
        }

        // Calculate circle MTV
        float len = glm::sqrt(len_squared);
        glm::vec3 circ_mtv = ((otherPos_xz - thisPos_xz) / len) * (radius_sum - len);

        // Calculate line MTV
        float aRight = (otherPos.y + other->getHeight()) - thisPos.y;
        float aLeft = (thisPos.y + m_height) - otherPos.y;
        glm::vec3 line_mtv = (aRight < aLeft) ? glm::vec3(0.0f, -aRight, 0.0f) : glm::vec3(0.0f, aLeft, 0.0f);

        // Choose smaller MTV to translate by
        glm::vec3 mtv = (glm::length2(circ_mtv) <= glm::length2(line_mtv)) ? circ_mtv : line_mtv;

        if (thisTrans->is_static()) {
            // If one of the colliders is static, we apply the full MTV to the other
            otherTrans->translate(mtv);
        } else if (otherTrans->is_static()) {
            thisTrans->translate(-mtv);
        } else {
            // Otherwise we split the MTV in half (ish) and apply to both
            glm::vec3 half_mtv = mtv / 2.0f;
            otherTrans->translate(half_mtv);
            thisTrans->translate(-half_mtv);
        }

        return true;
    }
    return false;
}

bool CollCylinder::collideComposite(CollComposite *other)
{
    return other->collideCylinder(this);
}

bool CollCylinder::collide(std::shared_ptr<CollisionVolume> other)
{
    return other->collideCylinder(this);
}

std::pair<glm::vec3, glm::vec3> CollCylinder::getExtrema()
{
    return std::make_pair(glm::vec3(-m_radius, 0.f, -m_radius) + m_localPos,
                          glm::vec3(m_radius, m_height, m_radius) + m_localPos);
}

float CollCylinder::intersect(glm::vec3 origin, glm::vec3 direction)
{
    return -1.0f; // TODO: Not yet implemented
}

void CollCylinder::drawWireframe()
{
    Graphics *g = Graphics::getGlobalInstance();
    auto thisTrans = m_owner->getSibling<CTransform>();

    g->clearTransform();
    g->translate(thisTrans->toWorld(m_localPos));
    g->scale(glm::vec3(m_radius, m_height, m_radius));
    g->drawShape("wire_cylinder");
}
