#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <QSet>
#include <QQueue>
#include <QMutex>
#include <QMultiHash>
#include <QLinkedList>
#include <memory>

#include "engine/systems/System.h"

// How many IDs to initially populate our queue with
#define NUM_STARTING_IDS 500

// How small our queue has to get before we decide to add more IDs
#define ID_REFILL_THRESH 50

class GameObject;
class UIObject;
class RaycastSystem;
class UISystem;

/**
 * @brief The GameWorld class represents an abstracted set of game
 * logic functions. Each GameWorld hosts a series of modular Systems,
 * which use data stored in Components attached to GameObjects to
 * update and draw the game. The GameWorld class itself hosts no
 * logic, but deals with the messy details of diverting new Components
 * to the right Systems and updating everything in the correct order.
 * GameWorld should never be subclassed.
 */
class GameWorld
{
public:
    GameWorld();
    ~GameWorld();

    /**
     * @brief registerForTick adds the given system to the GameWorld's
     *        list of systems to update on tick.
     * @param s - a shared pointer to a subclass of System
     */
    void registerForTick(std::shared_ptr<System> s);

    /**
     * @brief registerForDraw adds the given system to the GameWorld's
     *        list of systems to update on draw.
     * @param s - a shared pointer to a subclass of System
     */
    void registerForDraw(std::shared_ptr<System> s);

    /**
     * @brief registerUISystem adds the given system to the GameWorld's
     *        list of UI systems. UI systems both tick AND draw, but
     *        are only updated after all other systems have been dealt
     *        with.
     * @param s - a shared pointer to a subclass of UISystem
     */
    void registerUISystem(std::shared_ptr<UISystem> s);

    /**
     * @brief removeFromTick takes the given system out of the GameWorld's
     *        tick system list.
     * @param s - a shared pointer to a subclass of System. If s is not
     *            in the GameWorld's system list, the function will fail
     *            silently (no errors, and no impact on the application).
     */
    void removeFromTick(const std::shared_ptr<System> &s);

    /**
     * @brief removeFromDraw takes the given system out of the GameWorld's
     *        draw system list.
     * @param s - a shared pointer to a subclass of System. If s is not
     *            in the GameWorld's system list, the function will fail
     *            silently (no errors, and no impact on the application).
     */
    void removeFromDraw(const std::shared_ptr<System> &s);

    /**
     * @brief removeUISystem takes the given system out of the GameWorld's
     *        ui system list.
     * @param s - a shared pointer to a subclass of UISystem. If s is not
     *            in the GameWorld's system list, the function will fail
     *            silently (no errors, and no impact on the application).
     */
    void removeUISystem(const std::shared_ptr<UISystem> &s);

    /**
     * @brief getRaycaster gets the raycast handler for this GameWorld.
     *        Any System that wants to shoot a ray should use this.
     * @return a shared pointer to this GameWorld's raycast handler.
     */
    std::shared_ptr<RaycastSystem> getRaycaster();

    /**
     * @brief getNewObjID gets a uniquely identifying integer. This is
     *        used to generate unique IDs for GameObjects.
     * @return an integer ID currently not in use by any other object.
     */
    int getNewObjID();

    /**
     * @brief addGameObject adds the given GameObject to the GameWorld,
     *        automatically assigning its components to all appropriate
     *        systems.
     * @param g - a shared pointer to the object to add
     */
    void addGameObject(std::shared_ptr<GameObject> g);

    /**
     * @brief removeGameObject removes the given GameObject from the
     *        GameWorld. Note that this is NOT instantaneous - removed
     *        GameObjects are stored in a queue and deleted at the end
     *        of each tick.
     * @param g - a shared pointer to the object to delete
     */
    void removeGameObject(const std::shared_ptr<GameObject> &g);

    /**
     * @brief addUIObject adds the given UIObject to the GameWorld,
     *        automatically assigning its components to the appropriate
     *        systems.
     * @param o - a shared pointer to the object to add
     */
    void addUIObject(std::shared_ptr<UIObject> o);

    /**
     * @brief removeUIObject TODO: NOT YET IMPLEMENTED
     * @param o - a shared pointer to the object to remove
     */
    void removeUIObject(const std::shared_ptr<UIObject> o);

    /**
     * @brief tick is called as close to 60 times a second as possible.
     *        On each tick, all Systems that have been registered for
     *        Tick in this GameWorld will update their components,
     *        advancing the game logic.
     * @param seconds - the number of seconds that have passed between
     *        the previous tick and the current tick.
     */
    void tick(float seconds);

    /**
     * @brief draw tells all Systems registered for Draw to run their
     *        draw calls.
     */
    void draw();

    /**
     * @brief resize alerts every System (regardless of registration)
     *        that the window size has changed.
     * @param w - The new width of the window, in pixels
     * @param h - The new height of the window, in pixels
     */
    void resize(int w, int h);

private:
    // Adds additional IDs to the end of the ID queue
    void refillIDs();

    // ACTUALLY deletes all the GameObjects that registered for
    // deletion during the tick.
    void deleteGameObjects();

    /* These maps relate the type of a component (derived as a string
     * via typeid().name) to a list of System pointers. We use them
     * to assign components of newly-added GameObjects to the
     * appropriate systems. See .cpp comments for additional details.
     */
    QMultiHash<QString, std::shared_ptr<System>> m_tickSysMap;
    QMultiHash<QString, std::shared_ptr<System>> m_drawSysMap;
    QMultiHash<QString, std::shared_ptr<UISystem>> m_uiSysMap;

    /* In addition to the maps above, we also store an ordered list
     * of Systems for iterating over. We maintain this list in a
     * strictly sorted order, with low priority number systems
     * ticking/drawing before high priority number ones. This gives
     * us a deterministic, consistent update order.
     */
    QList<std::shared_ptr<System>> m_tickSysList;
    QList<std::shared_ptr<System>> m_drawSysList;
    QList<std::shared_ptr<UISystem>> m_uiSysList;
    // TODO/NOTE: technically this would be a lot faster with a
    // QLinkedList, but the removal from them is weirdly complicated.

    /* Raycasts are kind of weird. The raycaster needs to operate over
     * collision components like the CollisionSystem, but it doesn't
     * have anything to do each tick - it's only a utility that other
     * things can call. So we treat it as a special singleton system
     * that all GameWorlds have an instance of, and that any system
     * can access to do raycasts.
     */
    std::shared_ptr<RaycastSystem> m_raycaster;

    /* GameObjects are frequently added and removed from the world,
     * so we store them in a Set to facilitate fast insertion and
     * deletion. Uniqueness is determined by the object ID provided
     * by getNewObjID().
     */
    QSet<std::shared_ptr<GameObject>> m_gameObjects;

    /* Deleting a GameObject in the middle of a tick causes all
     * sorts of problems. So we queue GameObjects for removal
     * in this queue, then delete them once the tick ends.
     */
    QQueue<std::shared_ptr<GameObject>> m_toDelete;

    /* A queue containing free IDs for gameObjects.
     * By default, this is populated with numbers 1 through
     * NUM_STARTING_IDS and then refilled when its size falls
     * below ID_REFILL_THRESH. Additionally, when a gameObject
     * is deleted, its ID is freed as it returns to this Queue.
     */
    QQueue<uint> m_ids;
    QMutex m_id_mutex; // For thread safety on getID calls
    uint m_highest_id;
};

#endif // GAMEWORLD_H
