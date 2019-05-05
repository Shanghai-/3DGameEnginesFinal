#include "ProgressTracker.h"

#include "engine/components/CTransform.h"
#include "engine/components/CCollider.h"
#include "engine/components/volumes/CollCylinder.h"
#include "engine/objectManagement/GameWorld.h"
#include "vulpecula/responders/WinResp.h"

ProgressTracker::ProgressTracker(int priority, std::shared_ptr<AudioSystem> audioSys) :
    System(priority),
    m_audioSys(audioSys),
    m_totalStars(0),
    m_curStars(0)
{
}

ProgressTracker::~ProgressTracker()
{
}

QString ProgressTracker::getComponentType() const
{
    return typeid(CTransform).name();
}

void ProgressTracker::addComponent(const std::shared_ptr<Component> &c)
{
    if (c->getParent()->getName().contains("Zone")) {
        m_totalStars++;


    }
}

void ProgressTracker::removeComponent(const std::shared_ptr<Component> &c)
{
    if (c->getParent()->getName().contains("Zone")) {
        m_curStars++;

        switch (m_curStars) {
            case 2:
                m_audioSys->fadeChannelVolume("Music", 0.45f, 1.5f);
                m_audioSys->fadeChannelVolume("Ambient", 0.4f, 1.5f);
                break;
            case 3:
                m_audioSys->fadeChannelVolume("Music", 0.6f, 1.5f);
                m_audioSys->fadeChannelVolume("Ambient", 0.25f, 1.5f);
                break;
            case 4:
                m_audioSys->fadeChannelVolume("Music", 1.0f, 1.5f);
                m_audioSys->fadeChannelVolume("Ambient", 0.1f, 1.5f);
                break;
        }

        if (m_curStars == m_totalStars) {
            // Player won the game! start the endgame sequence
            // Places a trigger up on the cliff that, when touched, fades the screen to white

            auto winTrigger = std::make_shared<GameObject>("WinTrigger", m_gameWorld->getNewObjID());
            winTrigger->addComponent(std::make_shared<CTransform>(winTrigger, true, glm::vec3(0.0f, 21.0f, 37.0f)));
            auto coll = std::make_shared<CollCylinder>(glm::vec3(0.f, -10.f, 0.f), 20.0f, 3.5f);
            auto resp = std::make_shared<WinResp>(m_gameWorld);
            winTrigger->addComponent(std::make_shared<CCollider>(winTrigger, coll, true, resp));

            m_gameWorld->addGameObject(winTrigger);
        }
    }
}

void ProgressTracker::tick(float seconds)
{
    // TODO: Keep star counter UI element up to date
}
