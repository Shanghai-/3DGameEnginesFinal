#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "System.h"

#include "engine/components/CCollider.h"

#include <utility>

class Graphics;

class CollisionSystem : public System
{
public:
    CollisionSystem(int priority);
    ~CollisionSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);
    void draw();

private:
    QString m_type;
    QSet<std::shared_ptr<CCollider>> m_colliders;
    QMultiHash<std::shared_ptr<CCollider>, std::shared_ptr<CCollider>> m_currently_colliding;

    Graphics *m_graphics;
};

#endif // COLLISIONSYSTEM_H
