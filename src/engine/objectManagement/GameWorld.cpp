#include "GameWorld.h"

#include <assert.h>
#include <QHash>
#include <iostream>

#include "GameObject.h"
#include "engine/components/Component.h"
#include "engine/systems/RaycastSystem.h"
#include "engine/ui/UISystem.h"

GameWorld::GameWorld() :
    m_highest_id(1)
{
    // Rough guesstimates for how many of each system we'll need.
    // This is just a super minor optimization.
    m_tickSysList.reserve(8);
    m_drawSysList.reserve(3);
    m_uiSysList.reserve(5);

    // Instantiate and add the singleton Raycaster system.
    auto rs = std::make_shared<RaycastSystem>();
    rs->setParent(this);
    m_tickSysMap.insert(rs->getComponentType(), rs);
    m_raycaster = rs;

    // Reserve space for NUM_STARTING_IDS ints and then do the initial
    // queue population
    m_ids.reserve(NUM_STARTING_IDS);
    refillIDs();
}

GameWorld::~GameWorld()
{
}

// In order for the QHash class to work with a GameObject
// pointer as its key, we need to tell it how to hash a
// GameObject pointer. In this case, we just use its ID.
inline uint qHash(const std::shared_ptr<GameObject> &key)
{
    return key->getID();
}

void GameWorld::registerForTick(std::shared_ptr<System> s)
{
    // Give the system a pointer to this GameWorld
    s->setParent(this);

    // Add it to our system map according to the type it
    // requests. All Components added to the Gameworld
    // that match this type will be added to the System
    // as well.
    m_tickSysMap.insert(s->getComponentType(), s);

    // Insert the System into the tick list in according to
    // its priority. This is how we maintain deterministic
    // system update order.
    int idx = 0;
    QListIterator<std::shared_ptr<System>> it(m_tickSysList);
    while(it.hasNext()) {
        // When we find a system that has a greater priority
        // than the one we're trying to add, we stop looping
        // and insert it there.
        if (it.next()->getPriority() > s->getPriority()) {
            break;
        }
        idx++;
    }

    QSetIterator<std::shared_ptr<GameObject>> im(m_gameObjects);
    while(im.hasNext()) {
        std::shared_ptr<GameObject> g = im.next();

        QList<std::shared_ptr<Component>> components = g->getComponentList();
        QListIterator<std::shared_ptr<Component>> iz(components);
        while (iz.hasNext())
        {
            std::shared_ptr<Component> c = iz.next();

            QString componentType = typeid(*c).name();
            if (componentType == s->getComponentType()) {
                s->addComponent(c);
                c->addSystem(s.get());
            }
        }
    }

    m_tickSysList.insert(idx, s);
}

void GameWorld::registerForDraw(std::shared_ptr<System> s)
{
    // Give the system a pointer to this GameWorld
    s->setParent(this);

    // Add it to our system map. See registerForTick() for details.
    m_drawSysMap.insert(s->getComponentType(), s);

    // Insert the system into the list at the appropriate place.
    // Again, check registerForTick() for more details.
    int idx = 0;
    QListIterator<std::shared_ptr<System>> it(m_drawSysList);
    while(it.hasNext()) {
        if (it.next()->getPriority() > s->getPriority()) {
            break;
        }
        idx++;
    }

    m_drawSysList.insert(idx, s);
}

void GameWorld::registerUISystem(std::shared_ptr<UISystem> s)
{
    // Add the UI System to our system map. See registerForTick()
    // for more details.
    m_uiSysMap.insert(s->getComponentType(), s);

    // Insert the UI System into the list at the appropriate place.
    // Again, see registerForTick() for more details.
    int idx = 0;
    QListIterator<std::shared_ptr<UISystem>> it(m_uiSysList);
    while(it.hasNext()) {
        if (it.next()->getPriority() > s->getPriority()) {
            break;
        }
        idx++;
    }

    m_uiSysList.insert(idx, s);
}

void GameWorld::removeFromTick(const std::shared_ptr<System> &s)
{
    // Just remove the System from both the map and the list.
    // That's it.
    m_tickSysMap.remove(s->getComponentType(), s);
    m_tickSysList.removeOne(s);
}

void GameWorld::removeFromDraw(const std::shared_ptr<System> &s)
{
    // Remove the System from both the map and the list.
    m_drawSysMap.remove(s->getComponentType(), s);
    m_drawSysList.removeOne(s);
}

void GameWorld::removeUISystem(const std::shared_ptr<UISystem> &s)
{
    // Remove the System from both the map and the list.
    m_uiSysMap.remove(s->getComponentType(), s);
    m_uiSysList.removeOne(s);
}

std::shared_ptr<RaycastSystem> GameWorld::getRaycaster()
{
    return m_raycaster;
}

int GameWorld::getNewObjID()
{
    // We lock the mutex before messing with the ID queue
    // just in case multithreading ever becomes a thing
    QMutexLocker locker(&m_id_mutex);
    return m_ids.dequeue();
}

void GameWorld::addGameObject(std::shared_ptr<GameObject> g)
{
    // 0 should never be a valid ID for an added GameObject.
    // It is reserved for orphaned Components, and shouldn't
    // propagate this high up ever.
    assert(g->getID() != 0);
    m_gameObjects.insert(g);

    // Get all of the Components the GameObject has attached to it
    // and loop over them
    QList<std::shared_ptr<Component>> components = g->getComponentList();
    QListIterator<std::shared_ptr<Component>> it(components);
    while (it.hasNext())
    {
        std::shared_ptr<Component> toAdd = it.next();

        // We find the specific type string of the component by
        // calling typeid() on it and getting the name property.
        // This is a function automatically generated by the
        // compiler. For example, the class "Cat" would end up
        // with a string like Cat1359 (gcc adds numbers to the
        // end of the name)
        QString componentType = typeid(*toAdd).name();

        // Index into the system map using the component's type.
        // Because we use a MultiMap to allow several systems to
        // register for the same type, this is a list.
        QList<std::shared_ptr<System>> systems = m_tickSysMap.values(componentType);

        // Iterate over the system list and add the component
        // to each one.
        QListIterator<std::shared_ptr<System>> in(systems);
        while (in.hasNext()) {
            std::shared_ptr<System> thisSys = in.next();
            thisSys->addComponent(toAdd);
            toAdd->addSystem(thisSys.get());
        }

        // Repeat the same process, but with draw systems instead
        // of tick systems.
        systems = m_drawSysMap.values(componentType);

        QListIterator<std::shared_ptr<System>> iz(systems);
        while (iz.hasNext()) {
            std::shared_ptr<System> thisSys = iz.next();
            thisSys->addComponent(toAdd);
            toAdd->addSystem(thisSys.get());
        }
    }
}

void GameWorld::removeGameObject(const std::shared_ptr<GameObject> &g)
{
    m_toDelete.enqueue(g);
}

void GameWorld::addUIObject(std::shared_ptr<UIObject> o)
{
    // Very similar to addGameObject. We get the components, index into
    // the map, and add to all the resulting systems.
    QVectorIterator<std::shared_ptr<UIComponent>> it(o->getComponents());
    while (it.hasNext()) {
        std::shared_ptr<UIComponent> toAdd = it.next();
        QString requiredSystem = typeid(*toAdd).name();

        QList<std::shared_ptr<UISystem>> systems = m_uiSysMap.values(requiredSystem);

        QListIterator<std::shared_ptr<UISystem>> in(systems);
        while (in.hasNext()) {
            std::shared_ptr<UISystem> thisSys = in.next();
            thisSys->addComponent(toAdd);
        }
    }
}

void GameWorld::removeUIObject(const std::shared_ptr<UIObject> o)
{
    // TODO: NOT YET IMPLEMENTED
}

void GameWorld::deleteGameObjects()
{
    // Iterate over our queue of things to delete
    while (!m_toDelete.empty()) {
        std::shared_ptr<GameObject> g = m_toDelete.dequeue();

        // Take it out of the list of active GameObjects
        m_gameObjects.remove(g);

        // Kind of like when we add a GameObject: we get all of its components,
        // we loop over them, and we use them to index and find all systems that
        // are using them. Then we just remove them from those systems.
        QList<std::shared_ptr<Component>> components = g->getComponentList();
        QListIterator<std::shared_ptr<Component>> it(components);
        while (it.hasNext())
        {
            std::shared_ptr<Component> toRemove = it.next();
            toRemove->deleteSelf();

            /* std::shared_ptr<Component> toRemove = it.next();
            QString requiredSystem = typeid(*toRemove).name();

            QList<std::shared_ptr<System>> systems = m_tickSysMap.values(requiredSystem);

            QListIterator<std::shared_ptr<System>> in(systems);
            while (in.hasNext()) {
                std::shared_ptr<System> thisSys = in.next();
                thisSys->removeComponent(toRemove);
            }

            systems = m_drawSysMap.values(requiredSystem);

            QListIterator<std::shared_ptr<System>> iz(systems);
            while (iz.hasNext()) {
                std::shared_ptr<System> thisSys = iz.next();
                thisSys->removeComponent(toRemove);
            } */
        }

        m_ids.enqueue(g->getID());
    }
}

void GameWorld::tick(float seconds)
{
    // On each tick we start by updating the tick systems
    // This iterator goes in order down the list, so it's always
    // the same order every tick
    QListIterator<std::shared_ptr<System>> it(m_tickSysList);
    while (it.hasNext()) {
        std::shared_ptr<System> s = it.next();
        s->tick(seconds);
    }

    // Then once we've updated all the tick systems, we update
    // all the UI systems
    QListIterator<std::shared_ptr<UISystem>> im(m_uiSysList);
    while (im.hasNext()) {
        std::shared_ptr<UISystem> s = im.next();
        s->tick(seconds);
    }

    // Finally, we delete any GameObjects that queued themselves
    // to be deleted during the tick.
    if (!m_toDelete.empty()) deleteGameObjects();

    // Update elapsed time
    m_time += seconds;
}

void GameWorld::draw()
{
    // First we draw all of the draw systems in order according
    // to their priority
    QListIterator<std::shared_ptr<System>> it(m_drawSysList);
    while (it.hasNext()) {
        std::shared_ptr<System> s = it.next();
        s->draw();
    }

    // Then we draw all the UI systems. We need them to draw
    // over top of the game, so we have to draw them last.
    QListIterator<std::shared_ptr<UISystem>> im(m_uiSysList);
    while (im.hasNext()) {
        std::shared_ptr<UISystem> s = im.next();
        s->draw();
    }
}

void GameWorld::resize(int w, int h)
{
    // On resize, we notify all systems - tick, draw, and UI.

    QListIterator<std::shared_ptr<System>> it(m_tickSysList);
    while (it.hasNext()) {
        std::shared_ptr<System> s = it.next();
        s->resize(w, h);
    }

    QListIterator<std::shared_ptr<System>> iz(m_drawSysList);
    while (iz.hasNext()) {
        std::shared_ptr<System> s = iz.next();
        s->resize(w, h);
    }

    QListIterator<std::shared_ptr<UISystem>> im(m_uiSysList);
    while (im.hasNext()) {
        std::shared_ptr<UISystem> s = im.next();
        s->resize(w, h);
    }
}

float GameWorld::getElapsedTime()
{
    return m_time;
}

void GameWorld::refillIDs()
{
    // Lock the queue since we're messing with it
    QMutexLocker locker(&m_id_mutex);

    int i;
    for (i = 0; i < NUM_STARTING_IDS; i++)
    {
        int sum = m_highest_id + i;

        if (sum < 0) {
            // If our id ever wraps around to negative, we've
            // overflowed, which means we're functionally out of IDs.
            m_highest_id = 0;
            return;
        }
        m_ids.enqueue(sum);
    }
    m_highest_id += i;
}
