#include "CollBox.h"

#include "CollCylinder.h"
#include "CollSphere.h"
#include "CollComposite.h"
#include "engine/components/CTransform.h"
#include "engine/graphics/Graphics.h"
#include "glm/gtx/norm.hpp"
#include "engine/components/CCollider.h"

#include "glm/gtx/string_cast.hpp"
#include <iostream>

CollBox::CollBox(glm::vec3 localPos, glm::vec3 dimensions) :
    CollisionVolume(localPos),
    m_dims(dimensions)
{
}

CollBox::~CollBox()
{
}

bool CollBox::collideBox(CollBox *other)
{
    auto thisTrans = m_owner->getSibling<CTransform>();
    auto otherTrans = other->getOwner()->getSibling<CTransform>();

    glm::vec3 other_dims = other->getDimensions() / 2.0f;
    glm::vec3 my_dims = m_dims / 2.0f;

    glm::vec3 this_min = thisTrans->toWorld(m_localPos - my_dims);
    glm::vec3 this_max = thisTrans->toWorld(m_localPos + my_dims);
    glm::vec3 other_min = otherTrans->toWorld(other->getLocalPos() - other_dims);
    glm::vec3 other_max = otherTrans->toWorld(other->getLocalPos() + other_dims);

    bool x_intersect = (this_min.x <= other_max.x) && (this_max.x >= other_min.x);
    bool y_intersect = (this_min.y <= other_max.y) && (this_max.y >= other_min.y);
    bool z_intersect = (this_min.z <= other_max.z) && (this_max.z >= other_min.z);

    if (x_intersect && y_intersect && z_intersect) {

        // Calculate x MTV
        float xRight = (otherTrans->pos.x + other_dims.x) - (thisTrans->pos.x - my_dims.x);
        float xLeft = (thisTrans->pos.x + my_dims.x) - (otherTrans->pos.x - other_dims.x);
        glm::vec3 x_mtv = (xRight < xLeft) ? glm::vec3(-xRight, 0.0f, 0.0f) : glm::vec3(xLeft, 0.0f, 0.0f);
        float x_len = glm::length2(x_mtv);

        // Calculate y MTV
        float yRight = (otherTrans->pos.y + other_dims.y) - (thisTrans->pos.y - my_dims.y);
        float yLeft = (thisTrans->pos.y + my_dims.y) - (otherTrans->pos.y - other_dims.y);
        glm::vec3 y_mtv = (yRight < yLeft) ? glm::vec3(0.0f, -yRight, 0.0f) : glm::vec3(0.0f, yLeft, 0.0f);
        float y_len = glm::length2(y_mtv);

        // Calculate z MTV
        float zRight = (otherTrans->pos.z + other_dims.z) - (thisTrans->pos.z - my_dims.z);
        float zLeft = (thisTrans->pos.z + my_dims.z) - (otherTrans->pos.z - other_dims.z);
        glm::vec3 z_mtv = (zRight < zLeft) ? glm::vec3(0.0f, 0.0f, -zRight) : glm::vec3(0.0f, 0.0f, zLeft);
        float z_len = glm::length2(z_mtv);

        glm::vec3 mtv;
        float least = glm::min(glm::min(x_len, y_len), z_len);
        if (x_len == least) mtv = x_mtv;
        if (y_len == least) mtv = y_mtv;
        if (z_len == least) mtv = z_mtv;

        //std::cout << "mtv: " << glm::to_string(mtv) << std::endl;

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

bool CollBox::collideSphere(CollSphere *other)
{
    return other->collideBox(this);
}

bool CollBox::collideCylinder(CollCylinder *other)
{
    return other->collideBox(this);
}

bool CollBox::collideComposite(CollComposite *other)
{
    return other->collideBox(this);
}

bool CollBox::collide(std::shared_ptr<CollisionVolume> other)
{
    return other->collideBox(this);
}

std::pair<glm::vec3, glm::vec3> CollBox::getExtrema()
{
    return std::make_pair(m_localPos - (m_dims / 2.0f), m_localPos + (m_dims / 2.0f));
}

float CollBox::intersect(glm::vec3 origin, glm::vec3 direction)
{
    auto thisTrans = m_owner->getSibling<CTransform>();
    glm::vec3 min_pt = thisTrans->toWorld(m_localPos - (m_dims / 2.0f));
    glm::vec3 max_pt = thisTrans->toWorld(m_localPos + (m_dims / 2.0f));

    // Calculate min and max in x direction

    float t_min = (min_pt.x - origin.x) / direction.x;
    float t_max = (max_pt.x - origin.x) / direction.x;

    if (t_min > t_max) std::swap(t_min, t_max);

    float t_y_min = (min_pt.y - origin.y) / direction.y;
    float t_y_max = (max_pt.y - origin.y) / direction.y;

    if (t_y_min > t_y_max) std::swap(t_y_min, t_y_max);

    if (t_min > t_y_max || t_y_min > t_max) return -1;

    if (t_y_min > t_min) t_min = t_y_min;
    if (t_y_max < t_max) t_max = t_y_max;

    float t_z_min = (min_pt.z - origin.z) / direction.z;
    float t_z_max = (max_pt.z - origin.z) / direction.z;

    if (t_z_min > t_z_max) std::swap(t_z_min, t_z_max);

    if (t_min > t_z_max || t_z_min > t_max) return -1;

    if (t_z_min > t_min) t_min = t_z_min;
    if (t_z_max < t_max) t_max = t_z_max;

    if (t_min < 0.0f) {
        if (t_max < 0.0f) {
            return -1.0f;
        }
        return t_max;
    }
    return t_min;

    /*
    float t_min, t_max;
    if (inverse_direction.x >= 0) {
        t_min = (min_pt.x - origin.x) / inverse_direction.x;
        t_max = (max_pt.x - origin.x) / inverse_direction.x;
    } else {
        t_max = (min_pt.x - origin.x) / inverse_direction.x;
        t_min = (max_pt.x - origin.x) / inverse_direction.x;
    }

    // Min and max in y direction
    float t_y_min, t_y_max;
    if (inverse_direction.y >= 0) {
        t_y_min = (min_pt.y - origin.y) / inverse_direction.y;
        t_y_max = (max_pt.y - origin.y) / inverse_direction.y;
    } else {
        t_y_max = (min_pt.y - origin.y) / inverse_direction.y;
        t_y_min = (max_pt.y - origin.y) / inverse_direction.y;
    }

    // No intersection
    if (t_min > t_y_max || t_y_min > t_max) return -1.0f;

    // Update min and max if needed
    if (t_y_min > t_min) t_min = t_y_min;
    if (t_y_max < t_max) t_max = t_y_max;

    float t_z_min, t_z_max;
    if (inverse_direction.z >= 0) {
        t_z_min = (min_pt.z - origin.z) / inverse_direction.z;
        t_z_max = (max_pt.z - origin.z) / inverse_direction.z;
    } else {
        t_z_max = (min_pt.z - origin.z) / inverse_direction.z;
        t_z_min = (max_pt.z - origin.z) / inverse_direction.z;
    }

    // No intersection
    if (t_min > t_z_max || t_z_min > t_max) return -1.0f;

    if (t_z_min > t_min) t_min = t_z_min;
    if (t_z_max < t_max) t_max = t_z_max;

    if (t_min < 0.0f) {
        if (t_max < 0.0f) {
            std::cout << "shitty." << std::endl;
            return -1.0f;
        }
        return t_max;
    }
    return t_min;
    */
}

void CollBox::drawWireframe()
{
    Graphics *g = Graphics::getGlobalInstance();
    auto thisTrans = m_owner->getSibling<CTransform>();

    g->clearTransform();
    g->translate(thisTrans->toWorld(m_localPos));
    g->scale(m_dims);
    g->drawShape("wire_cube");
}

glm::vec3 CollBox::getDimensions()
{
    return m_dims;
}
