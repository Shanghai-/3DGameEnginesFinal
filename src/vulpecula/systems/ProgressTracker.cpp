#include "ProgressTracker.h"

#include "engine/components/CTransform.h"
#include "engine/components/CCollider.h"
#include "engine/components/CRenderable.h"
#include "engine/components/volumes/CollCylinder.h"
#include "engine/objectManagement/GameWorld.h"
#include "vulpecula/responders/WinResp.h"
#include "vulpecula/components/CStar.h"
#include "engine/graphics/Graphics.h"

ProgressTracker::ProgressTracker(int priority, AudioSystem *audioSys,
                                 Application *app,
                                 StarCountScript *starCounter) :
    System(priority),
    m_audioSys(audioSys),
    m_app(app),
    m_starCounter(starCounter),
    m_totalStars(0),
    m_curStars(0)
{
}

ProgressTracker::~ProgressTracker()
{
}

QString ProgressTracker::getComponentType() const
{
    return typeid(CStar).name();
}

void ProgressTracker::addComponent(const std::shared_ptr<Component> &c)
{
    m_totalStars++;
}

void ProgressTracker::removeComponent(const std::shared_ptr<Component> &c)
{
    m_curStars++;

    switch (m_curStars) {
    case 2:
        m_audioSys->fadeChannelVolume("Music", 0.45f, 1.5f);
        m_audioSys->fadeChannelVolume("Ambient", 0.4f, 1.5f);
        break;
    case 3:
        m_audioSys->fadeChannelVolume("Music", 0.57f, 1.5f);
        m_audioSys->fadeChannelVolume("Ambient", 0.25f, 1.5f);
        break;
    case 4:
        m_audioSys->fadeChannelVolume("Music", 0.71f, 1.5f);
        m_audioSys->fadeChannelVolume("Ambient", 0.1f, 1.5f);
        break;
    }

    m_starCounter->updateStarCount(m_curStars);

    if (m_curStars == m_totalStars) {
        // Player won the game! start the endgame sequence
        // Places a trigger up on the cliff that, when touched, fades the screen to white

        Graphics *g = Graphics::getGlobalInstance();
        g->addTexture("LightPillar", "/course/cs1950u/.archive/2019/student/vulpecula/env/light.png");
        Material m;
        m.useLighting = false;
        m.textureName = "LightPillar";
        g->addMaterial("LightPillar", m);

        auto winTrigger = std::make_shared<GameObject>("WinTrigger", m_gameWorld->getNewObjID());
        winTrigger->addComponent(std::make_shared<CTransform>(winTrigger, true,
                                                              glm::vec3(-3.40133f, 12.4939f, 33.3993f)));
        auto coll = std::make_shared<CollCylinder>(glm::vec3(0.f, -2.f, 0.f), 20.0f, 3.5f);
        auto resp = std::make_shared<WinResp>(m_gameWorld, m_app, m_audioSys);
        winTrigger->addComponent(std::make_shared<CCollider>(winTrigger, coll, true, resp));
        winTrigger->addComponent(std::make_shared<CRenderable>(winTrigger,
                                                               "/course/cs1950u/.archive/2019/student/vulpecula/env/LightPillar.obj", "LightPillar"));

        m_gameWorld->addGameObject(winTrigger);
    }
}
