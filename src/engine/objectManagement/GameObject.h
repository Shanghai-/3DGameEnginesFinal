#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QHash>
#include <QString>
#include <QSetIterator>
#include <memory>

class Component;
class GameWorld;

/**
 * @brief The GameObject class is a named collection of Components which makes up
 * one coherent entity in the game world. GameObjects are general containers for
 * specific Components - they should NOT be subclassed, as all entity-specific
 * data should be stored in custom components and manipulated by custom systems.
 */
class GameObject
{

public:
    /**
     * @brief GameObject constructs a new GameObject
     * @param name - an identifying string. This does NOT need to be unique; you
     *               can think of it as a label rather than a name.
     * @param id - a UNIQUE identifying number. Get by calling GameWorld::getNewObjID();
     */
    GameObject(QString name, const uint id);
    ~GameObject();

    template <class T>
    /**
     * @brief addComponent adds the given component to this GameObject's component
     *        map.
     * @param c - the component to add
     */
    void addComponent(const std::shared_ptr<T> c)
    {
        m_components.insert(typeid(T).name(), c);
    }

    template <class T>
    /**
     * @brief removeComponent removes the given component from this GameObject's
     *        component map.
     * @param c - a shared pointer to the component to remove
     */
    void removeComponent()
    {
        std::shared_ptr<Component> removed = m_components.take(typeid(T).name());
        finishRemoval(removed.get());
    }

    template <class T>
    /**
     * @brief getComponent gets a the component of the given type stored in the
     *        GameObject and returns it, properly casted to match the input type.
     * @return a shared pointer to the given Component type if this GameObject
     *         has a Component of that type, otherwise nullptr
     */
    std::shared_ptr<T> getComponent() const
    {
        return std::dynamic_pointer_cast<T>(m_components.value(typeid(T).name()));
    }

    /**
     * @brief getComponentList gets a list of all components contained in this
     *        GameObject. This is used to add components to Systems in GameWorld.
     * @return A list of un-casted, generic Component shared pointers.
     */
    QList<std::shared_ptr<Component>> getComponentList() const;

    /**
     * @brief getName gets the GameObject's name
     * @return a QString, which is the name
     */
    QString getName() const;

    /**
     * @brief getID gets the GameObject's unique ID. Useful for hash functions.
     * @return an unsigned integer ID.
     */
    uint getID() const;

    // Equality is determined by comparing unique IDs.
    bool operator==(const GameObject &other) const
    {
        return m_id == other.getID();
    }

private:
    void finishRemoval(Component *c);

    /* This name field is for GameObject identifiers like
     * "player" or "levelEndTrigger". By checking if the
     * name of the 'other' object in a collision is equal
     * to a certain value, we can react in different ways
     * to different colliders.
     */
    QString m_name;

    /* This id is unique to this GameObject only and is
     * passed on to all of its child components.
     */
    const uint m_id;

    /* Components are stored in a map indexed by their type name (as
     * defined by typeid().name, which produces a unique string for
     * each class at compile time). This allows us to request
     * components by type (getComponent<T>) rather than by an
     * arbitrary string name which leaves room for error and must be
     * cast by the caller.
     */
    QHash<QString, std::shared_ptr<Component>> m_components;
};

#endif // GAMEOBJECT_H
