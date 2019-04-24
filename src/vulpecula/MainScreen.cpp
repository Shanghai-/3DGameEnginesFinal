#include "MainScreen.h"

// ENGINE SYSTEMS
#include "engine/systems/RenderSystem.h"
#include "engine/systems/ThirdPersonCamSys.h"
#include "engine/systems/AudioSystem.h"
#include "engine/systems/CollisionSystem.h"
#include "engine/systems/AnimationSystem.h"

// ENGINE COMPONENTS
#include "engine/components/CCamera.h"
#include "engine/components/CTransform.h"
#include "engine/components/CRenderable.h"
#include "engine/components/CAudioSource.h"
#include "engine/components/CCollider.h"
#include "engine/components/CInputReceiver.h"
#include "engine/components/CAnimatedMesh.h"
#include "engine/components/cmeshcol.h"
#include "engine/components/ColEllipsoid.h"

// ENGINE MISC.
#include "engine/components/volumes/CollCylinder.h"
#include "engine/components/volumes/CollBox.h"
#include "engine/graphics/TextureCube.h"

// CUSTOM SYSTEMS
#include "vulpecula/systems/RandomAudioManager.h"
#include "vulpecula/systems/PlayerMovementSys.h"

// CUSTOM COMPONENTS
#include "vulpecula/components/RandomAudioSource.h"
#include "warmup1/CustomComponents/CPhysics.h"

// CUSTOM MISC.
#include "vulpecula/responders/GuitarZoneResp.h"
#include "vulpecula/responders/GuitarStarResp.h"

#include <assert.h>

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
    auto animSys = std::make_shared<AnimationSystem>(500);
    auto renderSys = std::make_shared<RenderSystem>(200);

    TextureCube::Cubemap skyMap = {"front.png", "back.png", "left.png", "right.png", "up.png", "down.png"};
    renderSys->setSkybox(std::make_shared<TextureCube>("/course/cs1950u/.archive/2019/student/vulpecula/skybox/", skyMap));

    // Renderer
    m_gw->registerForDraw(renderSys);
    m_gw->registerForDraw(animSys);

    // This displays wireframes around all colliders
    //m_gw->registerForDraw(collSys);

    // 3rd person camera system
    m_gw->registerForTick(std::make_shared<ThirdPersonCamSys>(400, 3.0f, 100.0f, 4.0f));

    // Non-environment collisions
    m_gw->registerForTick(collSys);

    // Animation bone updater
    m_gw->registerForTick(animSys);

    // Player input handling
    auto playSys = std::make_shared<PlayerMovementSys>(100);
    m_gw->registerForTick(playSys);
    //m_gw->registerForDraw(playSys);


    // Set up materials, lights, etc.
    loadGraphics();

    // Player setup
    std::shared_ptr<GameObject> player = std::make_shared<GameObject>("Player", m_gw->getNewObjID());
    //player->addComponent(std::make_shared<CTransform>(player, false, glm::vec3(0.01f, 0.0f, 0.01f), glm::vec3(0.0f), glm::vec3(0.6f, 0.6f, 1.8f)));
    player->addComponent(std::make_shared<CTransform>(player, false, glm::vec3(0.f, 15.f, 0.f), glm::vec3(0.0f), glm::vec3(0.2f)));
    player->addComponent(std::make_shared<CCamera>(player, glm::vec3(0.0f, 0.4f, 0.0f)));
    player->addComponent(std::make_shared<CAnimatedMesh>(player, "/course/cs1950u/.archive/2019/student/vulpecula/fox.fbx", "PureWhite"));
    auto coll = std::make_shared<CollCylinder>(glm::vec3(0.f, -0.375f, 0.f), 0.75f, 0.8f);
    auto comp = std::make_shared<CCollider>(player, coll, false);
    player->addComponent(comp);
    player->addComponent(std::make_shared<CInputReceiver>(player));
    player->addComponent(std::make_shared<ColEllipsoid>(player, glm::vec3(.75f, .5f, 1.25f)));
    player->addComponent(std::make_shared<CPhysics>(player, glm::vec3(0.f, -.3f, 0.f)));
    m_gw->addGameObject(player);

    // Load the ambient audio, set up channels, etc.
    initializeAudio(player);

    // Load in the map
    loadMap(playSys);
}

void MainScreen::loadGraphics()
{
    Graphics *g = Graphics::getGlobalInstance();

    Material fox;
    fox.useLighting = false;
    fox.color = glm::vec3(0.95f, 0.95f, 0.95f);
    fox.shaderName = "Animated";
    g->addMaterial("PureWhite", fox);

    Material noSnow;
    noSnow.useLighting = true;
    noSnow.color = glm::vec3(0.25f);//glm::vec3(0.1f);
    noSnow.shininess = 3.0f;
    g->addMaterial("Ground", noSnow);

    Material cave;
    cave.useLighting = true;
    cave.color = glm::vec3(0.1f, 0.8f, 0.9f) * 0.8f;
    g->addMaterial("Cave", cave);

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
    //directional.color = glm::vec3(0.7f, 0.77f, 0.9f) * 0.4f;
    directional.color = glm::vec3(0.7f, 0.77f, 0.9f);
    g->addLight(directional);

}

void MainScreen::loadMap(std::shared_ptr<PlayerMovementSys> playSys)
{
    std::shared_ptr<GameObject> terrain = std::make_shared<GameObject>("Ground", m_gw->getNewObjID());
    terrain->addComponent(std::make_shared<CTransform>(terrain, true, glm::vec3(0.0f, 0.f, 0.0f),
                                                       glm::vec3(0.f), glm::vec3(1.0f)));
    terrain->addComponent(std::make_shared<CRenderable>(terrain, ":/models/terrain.obj", "Ground"));
    std::shared_ptr<CMeshCol> terrainMesh = std::make_shared<CMeshCol>(terrain, ":/models/terrain.obj");
    terrain->addComponent(terrainMesh);
    playSys->addGlobalMesh(terrainMesh, glm::ivec2(0, 0));
    playSys->addMesh(glm::ivec2(0, 0));
    m_testMesh = terrainMesh;
    m_gw->addGameObject(terrain);

    std::shared_ptr<GameObject> cave = std::make_shared<GameObject>("Cave", m_gw->getNewObjID());
    cave->addComponent(std::make_shared<CTransform>(cave, true, glm::vec3(0.0f, 0.f, 0.0f),
                                                    glm::vec3(0.f), glm::vec3(1.0f)));
    cave->addComponent(std::make_shared<CRenderable>(cave, ":/models/cave.obj", "Cave"));
    std::shared_ptr<CMeshCol> caveMesh = std::make_shared<CMeshCol>(cave, ":/models/cave.obj");
    cave->addComponent(caveMesh);
    //playSys->addMesh(caveMesh);
    m_gw->addGameObject(cave);

    // Guitar stuff
    std::shared_ptr<GameObject> guitarZone = std::make_shared<GameObject>("GuitarZone", m_gw->getNewObjID());
    guitarZone->addComponent(std::make_shared<CTransform>(guitarZone, true, glm::vec3(-10.0f, 0.0f, -10.0f)));
    auto coll = std::make_shared<CollCylinder>(glm::vec3(0.f), 5.0f, 10.0f);
    QStringList guitarSounds = {":/sounds/guitar_01.ogg", ":/sounds/guitar_02.ogg", ":/sounds/guitar_03.ogg",
                                ":/sounds/guitar_04.ogg", ":/sounds/guitar_05.ogg", ":/sounds/guitar_06.ogg",
                                ":/sounds/guitar_07.ogg", ":/sounds/guitar_08.ogg", ":/sounds/guitar_09.ogg"};
    createAudioZone(guitarZone, guitarSounds, coll);

    std::shared_ptr<GameObject> guitarStar = std::make_shared<GameObject>("GuitarStar", m_gw->getNewObjID());
    guitarStar->addComponent(std::make_shared<CTransform>(guitarStar, true, glm::vec3(-10.0f, 1.0f, -15.0f),
                                                          glm::vec3(0.f), glm::vec3(0.5f)));
    createStar(guitarStar, ":/sounds/mus_guitar.ogg", guitarZone);

    // Piano stuff
    std::shared_ptr<GameObject> pianoZone = std::make_shared<GameObject>("PianoZone", m_gw->getNewObjID());
    pianoZone->addComponent(std::make_shared<CTransform>(pianoZone, true, glm::vec3(10.0f, 0.0f, 6.0f)));
    coll = std::make_shared<CollCylinder>(glm::vec3(0.f), 3.0f, 3.0f);
    QStringList pianoSounds = {":/sounds/piano_01.ogg", ":/sounds/piano_02.ogg", ":/sounds/piano_03.ogg",
                               ":/sounds/piano_04.ogg", ":/sounds/piano_05.ogg", ":/sounds/piano_06.ogg",
                               ":/sounds/piano_07.ogg"};
    createAudioZone(pianoZone, pianoSounds, coll);

    std::shared_ptr<GameObject> pianoStar = std::make_shared<GameObject>("PianoStar", m_gw->getNewObjID());
    pianoStar->addComponent(std::make_shared<CTransform>(pianoStar, true, glm::vec3(12.0f, 1.0f, 8.f),
                                                         glm::vec3(0.f, 6.0f, 0.f), glm::vec3(0.5f)));
    createStar(pianoStar, ":/sounds/mus_piano.ogg", pianoZone);

    // Woodwind stuff
    std::shared_ptr<GameObject> woodZone = std::make_shared<GameObject>("WoodZone", m_gw->getNewObjID());
    woodZone->addComponent(std::make_shared<CTransform>(woodZone, true, glm::vec3(3.0f, 0.0f, 9.0f)));
    coll = std::make_shared<CollCylinder>(glm::vec3(0.f), 3.0f, 3.0f);
    QStringList woodSounds = {":/sounds/woodwind_01.ogg", ":/sounds/woodwind_02.ogg",
                              ":/sounds/woodwind_03.ogg", ":/sounds/woodwind_04.ogg",
                              ":/sounds/woodwind_05.ogg", ":/sounds/woodwind_06.ogg"};
    createAudioZone(woodZone, woodSounds, coll);

    std::shared_ptr<GameObject> woodStar = std::make_shared<GameObject>("WoodStar", m_gw->getNewObjID());
    woodStar->addComponent(std::make_shared<CTransform>(woodStar, true, glm::vec3(0.5f, 1.0f, 10.0f),
                                                        glm::vec3(0.0f), glm::vec3(0.5f)));
    createStar(woodStar, ":/sounds/mus_woodwind.ogg", woodZone);
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
    audioSys->setChannelVolume("Music", 0.3f); // Music starts very quiet and gets louder with more tracks
    audioSys->setChannelVolume("SFX", 0.8f);
}

void MainScreen::createAudioZone(std::shared_ptr<GameObject> zoneObj, QStringList files, std::shared_ptr<CollisionVolume> vol)
{
    assert(!files.empty());
    assert(zoneObj != nullptr);
    assert(vol != nullptr);

    auto randomAudio = std::make_shared<RandomAudioSource>(zoneObj, files.first(), "SFX");
    randomAudio->setAmbient(true);
    randomAudio->setWaitInterval(3.0f, 7.0f);
    randomAudio->setStereoSpread(180);
    randomAudio->setVolume(0.7f);
    randomAudio->pause();

    for (int i = 1; i < files.size(); i++) {
        randomAudio->addFile(files.at(i));
    }

    zoneObj->addComponent(randomAudio);

    auto resp = std::make_shared<GuitarZoneResp>(randomAudio);
    auto comp = std::make_shared<CCollider>(zoneObj, vol, true, resp, CCollider::WORLD);
    // This isn't really necessary since all the static colliders ignore each other, but it's still good practice
    comp->ignoreLayer(CCollider::WORLD);
    zoneObj->addComponent(comp);
    m_gw->addGameObject(zoneObj);
}

void MainScreen::createStar(std::shared_ptr<GameObject> starObj, QString file, std::shared_ptr<GameObject> zone)
{
    assert(starObj != nullptr);
    //assert(!vol == nullptr);

    starObj->addComponent(std::make_shared<CRenderable>(starObj, ":/models/star.obj", "Star"));

    auto music = std::make_shared<CAudioSource>(starObj, file, false, "Music");
    music->setAmbient(true);
    music->setMuted(true);
    music->setStereoSpread(90);
    music->playLooping();
    starObj->addComponent(music);

    auto collider = std::make_shared<CollCylinder>(glm::vec3(0.f, -1.0f, 0.f), 2.0f, 1.0f);
    auto response = std::make_shared<GuitarStarResp>(starObj, zone, m_gw.get());
    auto component = std::make_shared<CCollider>(starObj, collider, true, response, CCollider::WORLD);
    component->ignoreLayer(CCollider::WORLD);
    starObj->addComponent(component);
    m_gw->addGameObject(starObj);
}

void MainScreen::tick(float seconds)
{
    m_gw->tick(seconds);
}

void MainScreen::draw()
{
    m_gw->draw();
    m_testMesh->drawWireframe();
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
