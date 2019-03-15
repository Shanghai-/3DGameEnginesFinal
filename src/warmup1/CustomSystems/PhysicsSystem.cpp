#include "PhysicsSystem.h"

#include "engine/components/CTransform.h"
#include "engine/util/CommonIncludes.h"

#define FLOOR_DRAG 6.0f

PhysicsSystem::PhysicsSystem(int priority) :
    System(priority)
{
}

PhysicsSystem::~PhysicsSystem()
{
}

QString PhysicsSystem::getComponentType() const
{
    return typeid(CPhysics).name();
}

inline uint qHash(const std::shared_ptr<CPhysics> &c) {
    return c->getId();
}

void PhysicsSystem::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CPhysics> phys = std::dynamic_pointer_cast<CPhysics>(c);
    m_components.insert(phys);
}

void PhysicsSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CPhysics> phys = std::dynamic_pointer_cast<CPhysics>(c);
    m_components.remove(phys);
}

void PhysicsSystem::tick(float seconds)
{
    QSetIterator<std::shared_ptr<CPhysics>> it(m_components);
    while (it.hasNext()) {
        std::shared_ptr<CPhysics> phys = it.next();
        //std::shared_ptr<CTransform> trans = phys->getParent()->getComponent<CTransform>();
        std::shared_ptr<CTransform> trans = phys->getSibling<CTransform>();

        // Apply drag - we want the object to slow down if it's not accelerating
        if (phys->vel.x > 0.f && phys->acc.x <= 0.f) {
            phys->vel.x -= FLOOR_DRAG * seconds;
            if (phys->vel.x < 0.f && phys->acc.x == 0.f) {
                phys->vel.x = 0.0f;
            }
        } else if (phys->vel.x < 0.f && phys->acc.x >= 0.f) {
            phys->vel.x += FLOOR_DRAG * seconds;
            if (phys->vel.x > 0.f && phys->acc.x == 0.f) {
                phys->vel.x = 0.0f;
            }
        }
        // TODO: Right now this adds a weird curve to objects as it overshoots in
        // one dimension but not the other, but it doesn't matter much really
        if (phys->vel.z > 0.f && phys->acc.z <= 0.f) {
            phys->vel.z -= FLOOR_DRAG * seconds;
            if (phys->vel.z < 0.f & phys->acc.z == 0.f) {
                phys->vel.z = 0.0f;
            }
        } else if (phys->vel.z < 0.f && phys->acc.z >= 0.f) {
            phys->vel.z += FLOOR_DRAG * seconds;
            if (phys->vel.z > 0.f && phys->acc.z == 0.f) {
                phys->vel.z = 0.0f;
            }
        }

        // Apply gravity if too high above floor
        if (trans->pos.y > 0.1f) {
            phys->acc.y = -9.8;
        }

        // We increase velocity by acceleration every tick, then change
        // position by velocity.
        phys->vel += phys->acc * seconds;
        trans->pos += glm::min(phys->vel, 12.0f) * seconds;

        // If the player is below the floor, place them at the floor.
        if (trans->pos.y < 0.f) {
            trans->pos.y = 0.f;
        }

        // If the player is at the floor, allow them to jump and reset their y motion.
        if (trans->pos.y == 0.f) {
            phys->canJump = true;
            phys->acc.y = 0.0f;
            phys->vel.y = 0.0f;
        }
    }
}
