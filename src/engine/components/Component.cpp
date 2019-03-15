#include "Component.h"

Component::Component(std::shared_ptr<GameObject> parent) :
    m_parent(parent)
{
    if (m_parent != nullptr) {
        // Inherit ID from parent
        m_id = m_parent->getID();

    } else {
        // Under normal circumstances, the ID should never be 0
        // This is exclusively for orphaned Components, and will
        // cause an assertion fail in GameWorld if this Component
        // is ever passed into a standard System.
        m_id = 0;
    }
}

std::shared_ptr<GameObject> Component::getParent()
{
    return m_parent;
}

uint Component::getId() const
{
    return m_id;
}
