#ifndef RAYCASTSYSTEM_H
#define RAYCASTSYSTEM_H

#include "System.h"
#include "engine/components/CCollider.h"
#include "glm/glm.hpp"

class GameObject;
class CCollider;

class RaycastSystem : public System
{
public:
    struct RaycastHit {
        RaycastHit(bool hit, std::shared_ptr<GameObject> obj, glm::vec3 norm) :
            didHit(hit), obj(obj), normal(norm) {}
        bool didHit;
        std::shared_ptr<GameObject> obj;
        glm::vec3 normal;
    };

    RaycastSystem();
    ~RaycastSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);

    RaycastHit cast(glm::vec3 origin, glm::vec3 direction);
    RaycastHit cast(glm::vec3 origin, glm::vec3 direction, int layer);
private:
    QSet<std::shared_ptr<CCollider>> m_colliders;
};

#endif // RAYCASTSYSTEM_H
