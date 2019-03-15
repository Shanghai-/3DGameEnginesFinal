#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>

#include "engine/objectManagement/GameObject.h"

/**
 * @brief The Component class represents a collection of related
 * data which, alongside a System, adds new functionality to a
 * GameObject. Components lack any tick/draw methods because they
 * should NOT alter world state - that is what Systems are for.
 * Components simply encapsulate data and provide getters/
 * setters where appropriate.
 */
class Component
{
public:
    /**
     * @brief Component constructs a new Component attached to the
     *        given parent object. The Component inherits its
     *        parent's ID.
     * @param parent - a shared pointer to the GameObject which
     *        contains this Component. If nullptr, the Component
     *        is considered orphaned, and will cause an assertion
     *        failure if the GameWorld ever attempts to use it.
     */
    Component(std::shared_ptr<GameObject> parent);
    virtual ~Component() {}

    /**
     * @brief getParent returns the GameObject's parent pointer.
     * @return a pointer to the GameObject which contains this
     *         Component.
     */
    std::shared_ptr<GameObject> getParent();

    template <class T>
    /**
     * @brief getSibling is a convenience function which is
     *        equivalent to calling getParent()->getComponent().
     * @return a shared pointer to a Component of type T if the
     *         GameObject has a Component of that type, otherwise
     *         nullptr.
     */
    std::shared_ptr<T> getSibling() const
    {
        return m_parent->getComponent<T>();
    }

    /**
     * @brief getId gets the Component's ID (which is the same
     *        as its parent's)
     * @return an unsigned integer ID, useful for hashing
     */
    uint getId() const;

private:
    uint m_id;
    std::shared_ptr<GameObject> m_parent;
};

#endif // COMPONENT_H
