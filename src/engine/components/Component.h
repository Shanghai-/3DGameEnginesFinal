#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>

#include "engine/objectManagement/GameObject.h"

class System;

/**
 * @brief The Component class represents a collection of related
 * data which, alongside a System, adds new functionality to a
 * GameObject. Components lack any tick/draw methods because they
 * should NOT alter world state - that is what Systems are for.
 * Components simply encapsulate data and provide getters/
 * setters where appropriate.
 */
class Component : public std::enable_shared_from_this<Component>
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

    /**
     * @brief addSystem adds the given system to this Component's
     *        list of systems it is registered to. This is only
     *        called by GameWorld when a Component is added to
     *        a system to make the relation symmetrical.
     */
    void addSystem(System *s);

    /**
     * @brief removeSystem removes the given system from this
     *        Component's list of systems it is registered to.
     *        This is only called by GameWorld when a System
     *        is deleted.
     */
    void removeSystem(System *s);

    /**
     * @brief deleteSelf removes this Component from every System
     *        currently tracking it.
     */
    void deleteSelf();

private:
    uint m_id;
    std::shared_ptr<GameObject> m_parent;
    QSet<System *> m_systems;
};

#endif // COMPONENT_H
