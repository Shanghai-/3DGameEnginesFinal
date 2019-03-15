#include "CollSphere.h"

#include "engine/components/CCollider.h"
#include "CollBox.h"
#include "CollComposite.h"
#include "engine/graphics/Graphics.h"
#include "glm/gtx/norm.hpp"

CollSphere::CollSphere(glm::vec3 localPos, float radius) :
    CollisionVolume(localPos),
    m_radius(radius)
{
    m_r2 = m_radius * m_radius;
}

CollSphere::~CollSphere()
{
}

bool CollSphere::collideBox(CollBox *other)
{
    auto thisTrans = m_owner->getSibling<CTransform>();
    auto otherTrans = other->getOwner()->getSibling<CTransform>();

    glm::vec3 box_dims = other->getDimensions();
    glm::vec3 box_min = otherTrans->toWorld(other->getLocalPos() + (box_dims / -2.0f));
    glm::vec3 box_max = otherTrans->toWorld(other->getLocalPos() + (box_dims / 2.0f));
    glm::vec3 sphere_center = thisTrans->toWorld(m_localPos);
    glm::vec3 closest = glm::vec3(glm::clamp(sphere_center.x, box_min.x, box_max.x),
                                  glm::clamp(sphere_center.y, box_min.y, box_max.y),
                                  glm::clamp(sphere_center.z, box_min.z, box_max.z));
    float len_squared = glm::length2(closest - sphere_center);

    if (len_squared <= m_r2) {
        // Skip MTV calculation for trigger intersections
        if (m_owner->isTrigger() || other->getOwner()->isTrigger()) {
            return true;
        }

        // Calculate MTV
        float len = glm::sqrt(len_squared);
        glm::vec3 mtv = ((closest - sphere_center) / len) * (m_radius - len);

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

bool CollSphere::collideSphere(CollSphere *other)
{
    return false;
}

bool CollSphere::collideCylinder(CollCylinder *other)
{
    return false;
}

bool CollSphere::collideComposite(CollComposite *other)
{
    return other->collideSphere(this);
}

bool CollSphere::collide(std::shared_ptr<CollisionVolume> other)
{
    return other->collideSphere(this);
}

std::pair<glm::vec3, glm::vec3> CollSphere::getExtrema()
{
    return std::make_pair(glm::vec3(-m_radius) + m_localPos, glm::vec3(m_radius) + m_localPos);
}

float CollSphere::intersect(glm::vec3 origin, glm::vec3 direction)
{
    return -1.0f; // TODO: Not yet implemented
}

void CollSphere::drawWireframe()
{
    Graphics *g = Graphics::getGlobalInstance();
    auto thisTrans = m_owner->getSibling<CTransform>();

    g->clearTransform();
    g->translate(thisTrans->toWorld(m_localPos));
    g->scale(glm::vec3(m_radius));
    g->drawShape("wire_sphere");
}
