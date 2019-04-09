#include "MainScreen.h"

#include "engine/systems/RenderSystem.h"
#include "engine/systems/ThirdPersonCamSys.h"
#include "engine/systems/AudioSystem.h"
#include "engine/systems/CollisionSystem.h"

#include "engine/components/CCamera.h"
#include "engine/components/CTransform.h"
#include "engine/components/CRenderable.h"
#include "engine/components/CAudioSource.h"
#include "engine/components/CCollider.h"
#include "engine/components/volumes/CollCylinder.h"
#include "engine/components/CInputReceiver.h"

#include "vulpecula/components/RandomAudioSource.h"

#include "vulpecula/systems/RandomAudioManager.h"
#include "vulpecula/systems/PlayerMovementSys.h"

#include "vulpecula/responders/GuitarZoneResp.h"

MainScreen::MainScreen(Application *parent) :
    m_parent(parent)
{
    m_gw = std::make_unique<GameWorld>();
    initializeGame();
}

MainScreen::~MainScreen()
{
}

void MainScreen::initializeGame()
{
    auto collSys = std::make_shared<CollisionSystem>(300);

    // Renderer
    m_gw->registerForDraw(std::make_shared<RenderSystem>(200));

    // This displays wireframes around all colliders
    m_gw->registerForDraw(collSys);

    // 3rd person camera system
    m_gw->registerForTick(std::make_shared<ThirdPersonCamSys>(400, 3.0f, 100.0f, 4.0f));

    // Non-environment collisions
    m_gw->registerForTick(collSys);

    // Player input handling
    m_gw->registerForTick(std::make_shared<PlayerMovementSys>(100));

    // Set up materials, lights, etc.
    loadGraphics();

    // Player setup
    std::shared_ptr<GameObject> player = std::make_shared<GameObject>("Player", m_gw->getNewObjID());
    player->addComponent(std::make_shared<CTransform>(player, false, glm::vec3(0.01f, 0.0f, 0.01f), glm::vec3(0.0f), glm::vec3(0.6f, 0.6f, 1.8f)));
    player->addComponent(std::make_shared<CCamera>(player, glm::vec3(0.0f, 0.4f, 0.0f)));
    player->addComponent(std::make_shared<CRenderable>(player, "cube", "PureWhite"));
    auto coll = std::make_shared<CollCylinder>(glm::vec3(0.f), 1.0f, 1.5f);
    auto comp = std::make_shared<CCollider>(player, coll, false);
    player->addComponent(comp);
    player->addComponent(std::make_shared<CInputReceiver>(player));
    m_gw->addGameObject(player);

    // Load the ambient audio, set up channels, etc.
    initializeAudio(player);

    // Load in the map
    loadMap();
}

void MainScreen::loadGraphics()
{
    Graphics *g = Graphics::getGlobalInstance();

    Material m;
    m.useLighting = false;
    m.color = glm::vec3(0.95f, 0.95f, 0.95f);
    g->addMaterial("PureWhite", m);

    Material noSnow;
    noSnow.useLighting = true;
    noSnow.color = glm::vec3(0.05f);
    g->addMaterial("Ground", noSnow);

    Material star;
    star.useLighting = true;
    star.color = glm::vec3(0.9f, 0.9f, 0.52549f);
    g->addMaterial("Star", star);

    Material debug_enter;
    debug_enter.useLighting = false;
    debug_enter.color = glm::vec3(0.1f, 1.0f, 0.3f);
    g->addMaterial("Debug_Enter", debug_enter);

    Light ambient;
    ambient.type = Light::LIGHT_TYPE::AMBIENT;
    ambient.color = glm::vec3(0.07f, 0.188f, 0.4588f) * 0.3f;
    g->addLight(ambient);

    Light directional;
    directional.type = Light::LIGHT_TYPE::DIRECTIONAL;
    directional.dir = glm::normalize(glm::vec3(0.2f, -0.7f, 0.1f));
    directional.color = glm::vec3(0.7f, 0.77f, 0.9f) * 0.4f;
    g->addLight(directional);
}

void MainScreen::loadMap()
{
    std::shared_ptr<GameObject> groundPlane = std::make_shared<GameObject>("Ground", m_gw->getNewObjID());
    groundPlane->addComponent(std::make_shared<CTransform>(groundPlane, true, glm::vec3(0.0f, -0.3f, 0.0f),
                                                           glm::vec3(0.0f), glm::vec3(40.0f, 1.0f, 40.0f)));
    groundPlane->addComponent(std::make_shared<CRenderable>(groundPlane, "quad", "Ground"));
    m_gw->addGameObject(groundPlane);


    std::shared_ptr<GameObject> guitarZone = std::make_shared<GameObject>("GuitarZone", m_gw->getNewObjID());
    guitarZone->addComponent(std::make_shared<CTransform>(guitarZone, true, glm::vec3(10.0f, 1.0f, 15.0f)));
    auto randomAudio = std::make_shared<RandomAudioSource>(guitarZone, ":/sounds/guitar_01.ogg", "Music");
    randomAudio->setAmbient(false);
    randomAudio->setWaitInterval(3.0f, 7.0f);
    randomAudio->addFile(":/sounds/guitar_02.ogg");
    randomAudio->addFile(":/sounds/guitar_03.ogg");
    randomAudio->addFile(":/sounds/guitar_04.ogg");
    randomAudio->addFile(":/sounds/guitar_05.ogg");
    randomAudio->addFile(":/sounds/guitar_06.ogg");
    randomAudio->addFile(":/sounds/guitar_07.ogg");
    randomAudio->addFile(":/sounds/guitar_08.ogg");
    randomAudio->addFile(":/sounds/guitar_09.ogg");
    randomAudio->setStereoSpread(90);
    randomAudio->setVolume(0.7f);
    randomAudio->pause();
    guitarZone->addComponent(randomAudio);
    auto coll = std::make_shared<CollCylinder>(glm::vec3(0.f, -2.0f, 0.f), 5.0f, 10.0f);
    auto resp = std::make_shared<GuitarZoneResp>(randomAudio);
    auto comp = std::make_shared<CCollider>(guitarZone, coll, true, resp, CCollider::WORLD);
    // This isn't really necessary since all the static colliders ignore each other, but it's still good practice
    comp->ignoreLayer(CCollider::WORLD);
    guitarZone->addComponent(comp);
    m_gw->addGameObject(guitarZone);

    std::shared_ptr<GameObject> guitarStar = std::make_shared<GameObject>("GuitarStar", m_gw->getNewObjID());
    guitarStar->addComponent(std::make_shared<CTransform>(guitarStar, true, glm::vec3(10.0f, 1.0f, 20.0f)));
    guitarStar->addComponent(std::make_shared<CRenderable>(guitarStar, ":/models/star.obj", "Star"));
    auto guitarMusic = std::make_shared<CAudioSource>(guitarStar, ":/sounds/mus_guitar.ogg", "Music");
    guitarMusic->setAmbient(true);
    guitarMusic->setMuted(true);
    guitarMusic->setStereoSpread(90);
    guitarMusic->playLooping();
    guitarStar->addComponent(guitarMusic);

    m_gw->addGameObject(guitarStar);
}

void MainScreen::initializeAudio(std::shared_ptr<GameObject> player)
{
    std::shared_ptr<AudioSystem> audioSys = std::make_shared<AudioSystem>(900, player);
    audioSys->createChannel("Ambient");
    audioSys->createChannel("Music");
    audioSys->createChannel("SFX");

    m_gw->registerForTick(audioSys);

    m_gw->registerForTick(std::make_shared<RandomAudioManager>(895, audioSys));

    // Create the wind sound emitter
    std::shared_ptr<GameObject> windEmitter = std::make_shared<GameObject>("WindEmitter", m_gw->getNewObjID());
    auto as = std::make_shared<CAudioSource>(windEmitter, ":/sounds/wind.ogg", false, "Ambient");
    as->setAmbient(true);
    as->playLooping();
    as->setStereoSpread(90.0f);
    as->setVolume(1.0f);
    windEmitter->addComponent(as);
    m_gw->addGameObject(windEmitter);

    // Create the chime sound emitter
    std::shared_ptr<GameObject> chimeEmitter = std::make_shared<GameObject>("ChimeEmitter", m_gw->getNewObjID());
    auto rnd = std::make_shared<RandomAudioSource>(chimeEmitter, ":/sounds/chm_md_01.ogg", "Ambient");
    rnd->setAmbient(true);
    rnd->setWaitInterval(6.0f, 20.0f);
    rnd->addFile(":/sounds/chm_md_02.ogg");
    rnd->addFile(":/sounds/chm_md_03.ogg");
    rnd->addFile(":/sounds/chm_md_04.ogg");
    rnd->addFile(":/sounds/chm_md_05.ogg");
    rnd->addFile(":/sounds/chm_md_06.ogg");
    rnd->addFile(":/sounds/chm_hi_01.ogg");
    rnd->addFile(":/sounds/chm_hi_02.ogg");
    rnd->addFile(":/sounds/chm_hi_03.ogg");
    rnd->addFile(":/sounds/chm_hi_04.ogg");
    rnd->addFile(":/sounds/chm_hi_05.ogg");
    rnd->addFile(":/sounds/chm_hi_06.ogg");
    rnd->setVolume(0.8f);
    rnd->play();
    chimeEmitter->addComponent(rnd);
    m_gw->addGameObject(chimeEmitter);

    audioSys->setChannelVolume("Ambient", 0.5f);
}

void MainScreen::tick(float seconds)
{
    m_gw->tick(seconds);
}

void MainScreen::draw()
{
    m_gw->draw();
}

void MainScreen::resize(int w, int h)
{
    m_gw->resize(w, h);
}

void MainScreen::onFocusGained()
{
}

void MainScreen::onFocusLost()
{
}

void MainScreen::onKeyPressed(QKeyEvent *event){}
void MainScreen::onKeyHeld(QKeyEvent *event){}
void MainScreen::onKeyReleased(QKeyEvent *event){}
void MainScreen::onMousePressed(QMouseEvent *event){}
void MainScreen::onMouseReleased(QMouseEvent *event){}
void MainScreen::onMouseMoved(QMouseEvent *event, int deltaX, int deltaY){}
void MainScreen::onScroll(QWheelEvent *event){}
