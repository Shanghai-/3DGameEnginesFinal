#include "CollComposite.h"

#include "engine/components/CTransform.h"
#include "engine/components/CCollider.h"

CollComposite::CollComposite() :
    CollisionVolume(glm::vec3(0.0f)),
    min_x(std::numeric_limits<float>::infinity()),
    min_y(std::numeric_limits<float>::infinity()),
    min_z(std::numeric_limits<float>::infinity()),
    max_x(-std::numeric_limits<float>::infinity()),
    max_y(-std::numeric_limits<float>::infinity()),
    max_z(-std::numeric_limits<float>::infinity())
{
}

CollComposite::~CollComposite()
{
    m_volumes.clear();
}

bool CollComposite::collideBox(CollBox *other)
{
    bool any = false;

    QVectorIterator<std::shared_ptr<CollisionVolume>> it(m_volumes);
    while (it.hasNext()) {
        if (it.next()->collideBox(other)) any = true;
    }
    return any;
}

bool CollComposite::collideSphere(CollSphere *other)
{
    bool any = false;

    QVectorIterator<std::shared_ptr<CollisionVolume>> it(m_volumes);
    while (it.hasNext()) {
        if (it.next()->collideSphere(other)) any = true;
    }
    return any;
}

bool CollComposite::collideCylinder(CollCylinder *other)
{
    bool any = false;

    QVectorIterator<std::shared_ptr<CollisionVolume>> it(m_volumes);
    while (it.hasNext()) {
        if (it.next()->collideCylinder(other)) any = true;
    }
    return any;
}

bool CollComposite::collideComposite(CollComposite *other)
{
    bool any = false;

    QVectorIterator<std::shared_ptr<CollisionVolume>> it(m_volumes);
    while (it.hasNext()) {
        if (it.next()->collideComposite(other)) any = true;
    }
    return any;
}

bool CollComposite::collide(std::shared_ptr<CollisionVolume> other)
{
    std::pair<glm::vec3, glm::vec3> other_bounds = other->getExtrema();
    std::pair<glm::vec3, glm::vec3> this_bounds = getExtrema();

    auto thisTrans = m_owner->getSibling<CTransform>();
    auto otherTrans = other->getOwner()->getSibling<CTransform>();

    glm::vec3 other_min = otherTrans->pos + other_bounds.first;
    glm::vec3 other_max = otherTrans->pos + other_bounds.second;
    glm::vec3 this_min = thisTrans->pos + this_bounds.first;
    glm::vec3 this_max = thisTrans->pos + this_bounds.second;

    bool x_intersect = (this_min.x <= other_max.x) && (this_max.x >= other_min.x);
    bool y_intersect = (this_min.y <= other_max.y) && (this_max.y >= other_min.y);
    bool z_intersect = (this_min.z <= other_max.z) && (this_max.z >= other_min.z);

    if (x_intersect && y_intersect && z_intersect) {
        return other->collideComposite(this);
    }
    return false;

    //return other->collideComposite(this);
}

std::pair<glm::vec3, glm::vec3> CollComposite::getExtrema()
{
    return std::make_pair(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, max_y, max_z));
}

void CollComposite::setOwner(CCollider *o)
{
    QVectorIterator<std::shared_ptr<CollisionVolume>> it(m_volumes);
    while (it.hasNext()) {
        it.next()->setOwner(o);
    }
    m_owner = o;
}

float CollComposite::intersect(glm::vec3 origin, glm::vec3 direction)
{
    return -1.0f; // TODO: Not yet implemented.
}

void CollComposite::drawWireframe()
{
    QVectorIterator<std::shared_ptr<CollisionVolume>> it(m_volumes);
    while (it.hasNext()) {
        it.next()->drawWireframe();
    }
}

void CollComposite::addCollider(const std::shared_ptr<CollisionVolume> &c)
{
    std::pair<glm::vec3, glm::vec3> bounds = c->getExtrema();
    glm::vec3 first = bounds.first + m_localPos;
    glm::vec3 second = bounds.second + m_localPos;

    if (first.x < min_x) min_x = first.x;
    if (first.y < min_y) min_y = first.y;
    if (first.z < min_z) min_z = first.z;
    if (second.x > max_x) max_x = second.x;
    if (second.y > max_y) max_y = second.y;
    if (second.z > max_z) max_z = second.z;

    m_volumes.append(c);
}
