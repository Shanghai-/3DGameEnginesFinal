#ifndef SYSTEM_H
#define SYSTEM_H

#include <QString>
#include <memory>

class Component;
class GameWorld;

/**
 * @brief The System class represents a compartmentalized piece of
 * game logic that operates over a collection of Components and
 * uses that data to change something about the state of the world.
 * Systems should NEVER store states or data - that's what Components
 * are for. By default, this base class provides empty implementations
 * for tick, draw, and resize. Subclasses are free to override whichever
 * ones they need to do their function.
 */
class System
{
public:
    System(int priority) : m_priority(priority) {}
    ~System() {}

    /**
     * @brief getPriority gets an integer used to determine when in
     *        the order this System should update. Lower priorities
     *        mean earlier execution.
     * @return an integer representing the system's priority.
     */
    int getPriority() const { return m_priority; }

    /**
     * @brief setParent sets the System's parent pointer to the
     *        given GameWorld.
     * @param gw - the GameWorld that the System is attached to
     */
    void setParent(GameWorld *gw) { m_gameWorld = gw; }

    /**
     * @brief getComponentType gets a class string indicating what
     *        type of component this system operates over. This should
     *        be implemented in subclasses using typeid().name().
     *        For example, if your System uses CoolComponent, you
     *        would simply write "return typeid(CoolComponent).name()"
     * @return The classname string of the component class this System
     *         operates over.
     */
    virtual QString getComponentType() const = 0;

    /**
     * @brief addComponent adds the given component to the System's
     *        Component tracking mechanism, whatever that may be.
     *        Typically it is implemented as a QSet, and Components
     *        are cast to the proper type before being added.
     * @param c - the Component to add to the System
     */
    virtual void addComponent(const std::shared_ptr<Component> &c) = 0;

    /**
     * @brief removeComponent removes the given component from the
     *        System's tracker.
     * @param c - the Component to remove
     */
    virtual void removeComponent(const std::shared_ptr<Component> &c) = 0;

    /**
     * @brief tick runs 60-ish times a second and updates the world.
     *        Override and implement this if your System will make
     *        changes to the world's state.
     * @param seconds - the number of seconds between the last tick
     *        and the current one
     */
    virtual void tick(float seconds) {}

    /**
     * @brief draw displays things in the world. Override and implement
     * this if your System will render anything. NOTE: draw() should
     * ONLY deal with visual output - any changes to world state (e.g.
     * moving an object around, deleting an object, etc.) must be
     * handled in tick().
     */
    virtual void draw() {}

    /**
     * @brief resize is called every time the window changes size.
     * @param w - the new width of the window, in pixels
     * @param h - the new height of the window, in pixels
     */
    virtual void resize(int w, int h) {}

protected:
    GameWorld *m_gameWorld;

private:
    const int m_priority;
};

#endif // SYSTEM_H
