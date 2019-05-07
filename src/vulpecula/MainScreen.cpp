#include "MainScreen.h"

// ENGINE SYSTEMS
#include "engine/systems/RenderSystem.h"
#include "engine/systems/ThirdPersonCamSys.h"
#include "engine/systems/AudioSystem.h"
#include "engine/systems/CollisionSystem.h"
#include "engine/systems/AnimationSystem.h"
#include "engine/systems/networksystem.h"

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

// UI IMPORTS
#include "engine/ui/UIScriptSys.h"
#include "engine/ui/UIDrawer.h"

// ENGINE MISC.
#include "engine/components/volumes/CollCylinder.h"
#include "engine/components/volumes/CollBox.h"
#include "engine/graphics/TextureCube.h"
#include "engine/frame/Application.h"
#include "view.h"

// CUSTOM SYSTEMS
#include "vulpecula/systems/RandomAudioManager.h"
#include "vulpecula/systems/PlayerMovementSys.h"
#include "vulpecula/systems/ProgressTracker.h"
#include "vulpecula/systems/particlesys.h"

// CUSTOM COMPONENTS
#include "vulpecula/components/RandomAudioSource.h"
#include "warmup1/CustomComponents/CPhysics.h"
#include "vulpecula/responders/waterresponse.h"

// CUSTOM MISC.
#include "vulpecula/responders/GuitarZoneResp.h"
#include "vulpecula/responders/GuitarStarResp.h"
#include "vulpecula/responders/Standable.h"
#include "vulpecula/responders/LowpassResp.h"

#include <glm/gtx/rotate_vector.hpp>
#include <assert.h>

MainScreen::MainScreen(Application *parent, bool isServer) :
    m_parent(parent),
    m_isServer(isServer)
{
    m_gw = std::make_unique<GameWorld>();
    initializeGame();
}

MainScreen::~MainScreen()
{
}

void MainScreen::initializeGame()
{
    auto netSys = std::make_shared<NetworkSystem>(400, m_gw.get(), m_isServer);
    auto collSys = std::make_shared<CollisionSystem>(300);
    auto animSys = std::make_shared<AnimationSystem>(500);
    auto renderSys = std::make_shared<RenderSystem>(200);
    auto partSys = std::make_shared<ParticleSys>(100);

    TextureCube::Cubemap skyMap = {"front.png", "back.png", "left.png", "right.png", "up.png", "down.png"};
    renderSys->setSkybox(std::make_shared<TextureCube>("/course/cs1950u/.archive/2019/student/vulpecula/skybox/", skyMap));

    // Renderer
    m_gw->registerForDraw(renderSys);
    m_gw->registerForDraw(animSys);

    // This displays wireframes around all colliders
    //m_gw->registerForDraw(collSys);
    m_gw->registerForDraw(partSys);
    m_gw->registerForTick(partSys);

    // 3rd person camera system
    m_gw->registerForTick(std::make_shared<ThirdPersonCamSys>(401, 3.0f, 100.0f, 4.0f));

    // Non-environment collisions
    m_gw->registerForTick(collSys);

    // Animation bone updater
    m_gw->registerForTick(animSys);

    // Player input handling
    auto playSys = std::make_shared<PlayerMovementSys>(100);
    m_gw->registerForTick(playSys);
    //m_gw->registerForDraw(playSys);

    // Networking
    m_gw->registerForTick(netSys);

    // UI Systems
    m_gw->registerUISystem(std::make_shared<UIScriptSys>(100));
    m_gw->registerUISystem(std::make_shared<UIDrawer>(200));


    // Set up materials, lights, etc.
    loadGraphics();

    // Player setup
    std::shared_ptr<GameObject> player = std::make_shared<GameObject>("Player", m_gw->getNewObjID());
    player->addComponent(std::make_shared<CTransform>(player, false, glm::vec3(0.f, 22.0f, 50.0f), glm::vec3(0.0f), glm::vec3(0.2f)));
    player->addComponent(std::make_shared<CCamera>(player, glm::vec3(0.0f, 0.4f, 0.0f)));
    //player->addComponent(std::make_shared<CAnimatedMesh>(player, "/course/cs1950u/.archive/2019/student/vulpecula/fox.fbx", "PureWhite"));
    player->addComponent(std::make_shared<CRenderable>(player, "cube", "Star"));
    auto coll = std::make_shared<CollCylinder>(glm::vec3(0.f, -0.375f, 0.f), 0.75f, 0.8f);
    auto comp = std::make_shared<CCollider>(player, coll, false);
    player->addComponent(comp);
    player->addComponent(std::make_shared<CInputReceiver>(player));
    player->addComponent(std::make_shared<ColEllipsoid>(player, glm::vec3(.5f, .5f, .5f)));
    player->addComponent(std::make_shared<CPhysics>(player, glm::vec3(0.f, -.2f, 0.f)));
    m_gw->addGameObject(player);

    // Load the ambient audio, set up channels, etc.
    initializeAudio(player);

    // Load in the map
    loadMap(playSys);
}

void MainScreen::loadGraphics()
{
    Graphics *g = Graphics::getGlobalInstance();

    // SHADERS

    g->addShader("Water", ":/shaders/water.vert", ":/shaders/water.frag");

    // MATERIALS

    Material fox;
    fox.useLighting = false;
    fox.color = glm::vec3(0.95f, 0.95f, 0.95f);
    fox.shaderName = "Animated";
    g->addMaterial("PureWhite", fox);

    Material noSnow;
    noSnow.useLighting = true;
    noSnow.color = glm::vec3(0.2f);//glm::vec3(0.1f);
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

    Material water;
    water.shaderName = "Water";
    g->addMaterial("Water", water);

    Material powerLine;
    powerLine.useLighting = true;
    powerLine.color = glm::vec3(0.07f, 0.07f, 0.07f);
    g->addMaterial("PowerLine", powerLine);

    g->addTexture("TowerTex", ":/images/pylon.png");
    Material powerTower;
    powerTower.useLighting = true;
    powerTower.textureName = "TowerTex";
    g->addMaterial("PowerTower", powerTower);

    Material darkGrey;
    darkGrey.useLighting = true;
    darkGrey.color = glm::vec3(0.4f);
    g->addMaterial("DarkGrey", darkGrey);

    darkGrey.color = glm::vec3(0.7f);
    g->addMaterial("LightGrey", darkGrey);

    darkGrey.color = glm::vec3(0.3f);
    g->addMaterial("DeadTrees", darkGrey);

    Material pine;
    pine.useLighting = true;
    pine.color = glm::vec3(0.28235, 0.4745, 0.1686);
    pine.shininess = 0.2;
    g->addMaterial("Pine", pine);

    pine.color = glm::vec3(0.34117, 0.30196, 0.21176);
    g->addMaterial("FallenTree", pine);

    g->addTexture("ChainLink", ":/images/chainlink.png");
    Material fencing;
    fencing.useLighting = true;
    fencing.textureName = "ChainLink";
    fencing.textureRepeat = glm::vec2(8.0f, 8.0f);
    g->addMaterial("ChainLink", fencing);

    // UI MATERIALS

    Material fade;
    fade.useLighting = false;
    fade.alpha = 0.0f;
    fade.color = glm::vec3(1.0f);
    g->addMaterial("WhiteFade", fade);

    Material pureBlack;
    pureBlack.useLighting = false;
    pureBlack.color = glm::vec3(0.0f);
    g->addMaterial("UIBlack", pureBlack);

    // LIGHTS

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
    // Terrain loading
    loadTerrain(playSys);

    // Power towers & lines, trees, water, etc.
    loadDecorations();

    // Stars
    loadObjectives();
}

void MainScreen::initializeAudio(std::shared_ptr<GameObject> player)
{
    std::shared_ptr<AudioSystem> audioSys = std::make_shared<AudioSystem>(900, player);
    audioSys->createChannel("Ambient");
    audioSys->createChannel("Music");
    audioSys->createChannel("SFX");

    m_gw->registerForTick(audioSys);

    m_gw->registerForTick(std::make_shared<RandomAudioManager>(895, audioSys));

    m_gw->registerForTick(std::make_shared<ProgressTracker>(905, audioSys));

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

    std::shared_ptr<GameObject> lowpassTrigger = std::make_shared<GameObject>("Lowpass", m_gw->getNewObjID());
    auto lowpassTrans = std::make_shared<CTransform>(lowpassTrigger, true, glm::vec3(-18.8043, -2.29797, 52.7059));
    lowpassTrigger->addComponent(lowpassTrans);
    auto lowColl = std::make_shared<CollBox>(glm::vec3(0.0f), glm::vec3(1.0, 9.68446, 13.3109));
    auto lowResp = std::make_shared<LowpassResp>(lowpassTrans->pos, audioSys.get());
    lowpassTrigger->addComponent(std::make_shared<CCollider>(lowpassTrigger, lowColl, true, lowResp));
    m_gw->addGameObject(lowpassTrigger);
}

void MainScreen::loadTerrain(std::shared_ptr<PlayerMovementSys> playSys)
{
    Graphics *g = Graphics::getGlobalInstance();

    glm::vec3 basePoint = glm::vec3(-62.0f, 0.0f, 65.0f);
    float r_add = 0.0f;
    std::string baseFile = "/course/cs1950u/.archive/2019/student/vulpecula/terrain/";
    int num_splits = 10;

    for (int r = 0; r < num_splits; r++) {
        float c_add = 0.0f;
        for (int c = 0; c < num_splits; c++) {
            std::string name_concat = "Terrain" + std::to_string(r) + std::to_string(c);
            std::string filename_concat = baseFile + name_concat + ".obj";
            QString name(name_concat.data());
            QString filename(filename_concat.data());

            if (r < 3) {
                std::string texture_concat = baseFile + name_concat + ".png";
                g->addTexture(name_concat, texture_concat);
                Material m;
                m.useLighting = true;
                m.textureName = name_concat;
                g->addMaterial(name_concat, m);
            }

            glm::vec3 center = basePoint + glm::vec3(c_add, 0, r_add);
            //std::cout << center.x << ", " << center.y << ", " << center.z << std::endl;

            std::shared_ptr<GameObject> ground = std::make_shared<GameObject>(name, m_gw->getNewObjID());
            ground->addComponent(std::make_shared<CTransform>(ground, true, center));
            //ground->addComponent(std::make_shared<CRenderable>(ground, filename, "Ground"));
            if (r < 3) {
                ground->addComponent(std::make_shared<CRenderable>(ground, filename, name));
            } else {
                ground->addComponent(std::make_shared<CRenderable>(ground, filename, "Ground"));
            }

            std::shared_ptr<CMeshCol> collider = std::make_shared<CMeshCol>(ground, filename);
            ground->addComponent(collider);

            playSys->addGlobalMesh(collider, glm::ivec2(c, r));

            if (c >= 3 && c <= 5 && r <= 2) {
                playSys->addMesh(glm::ivec2(c, r));
            }

            m_gw->addGameObject(ground);
            c_add += 16.0f;
        }
        r_add -= 16.0f;
    }

    // Cave stuff
    std::shared_ptr<GameObject> cave = std::make_shared<GameObject>("Cave", m_gw->getNewObjID());
    cave->addComponent(std::make_shared<CTransform>(cave, true, glm::vec3(0.0f, 0.f, 0.0f),
                                                    glm::vec3(0.f), glm::vec3(1.0f)));
    cave->addComponent(std::make_shared<CRenderable>(cave, QString((baseFile + "cave.obj").data()), "Cave"));
    std::shared_ptr<CMeshCol> caveMesh = std::make_shared<CMeshCol>(cave, QString((baseFile + "cave.obj").data()));
    cave->addComponent(caveMesh);
    playSys->addGlobalMesh(caveMesh, glm::ivec2(-1, -1));
    playSys->addMesh(glm::ivec2(-1, -1));
    m_gw->addGameObject(cave);

    baseFile = "/course/cs1950u/.archive/2019/student/vulpecula/env/";

    // Fallen tree and rock
    std::shared_ptr<GameObject> tree = std::make_shared<GameObject>("FallenTree", m_gw->getNewObjID());
    auto treeTrans = std::make_shared<CTransform>(tree, true, glm::vec3(-25.9757, -4.91587, -35.09472));
    tree->addComponent(treeTrans);
    tree->addComponent(std::make_shared<CRenderable>(tree, QString((baseFile + "FallenTree.obj").data()), "FallenTree"));
    auto treeColl = std::make_shared<CollBox>(glm::vec3(-7.0f, -6.0f, 8.0f), glm::vec3(25.0f, 2.0f, 16.0f));
    auto treeResp = std::make_shared<Standable>(treeTrans.get(), glm::vec3(-7.0f, -5.0f, 8.0f));
    tree->addComponent(std::make_shared<CCollider>(tree, treeColl, false, treeResp));
    m_gw->addGameObject(tree);

    std::shared_ptr<GameObject> rock = std::make_shared<GameObject>("RiverRock", m_gw->getNewObjID());
    rock->addComponent(std::make_shared<CTransform>(rock, true, glm::vec3(-39.91050, -8.77608, -20.63535),
                                                    glm::vec3(0, glm::radians(-106.0f), 0), glm::vec3(1)));
    rock->addComponent(std::make_shared<CRenderable>(rock, QString((baseFile + "RiverRock.obj").data()), "LightGrey"));
    m_gw->addGameObject(rock);

    // House and collapsed stone wall
    g->addTexture("House", baseFile + "house.png");
    Material mat;
    mat.useLighting = true;
    mat.textureName = "House";
    g->addMaterial("House", mat);

    std::shared_ptr<GameObject> house = std::make_shared<GameObject>("House", m_gw->getNewObjID());
    house->addComponent(std::make_shared<CTransform>(house, true, glm::vec3(66.187, 4.69492, -72.675),
                                                     glm::vec3(0, glm::radians(208.0f), 0), glm::vec3(1.858f)));
    house->addComponent(std::make_shared<CRenderable>(house, (baseFile + "House.obj").data(), "House"));
    m_gw->addGameObject(house);

    std::shared_ptr<GameObject> wall = std::make_shared<GameObject>("StoneWall", m_gw->getNewObjID());
    wall->addComponent(std::make_shared<CTransform>(wall, true, glm::vec3(58.547, 4.14696, -55.6984)));
    wall->addComponent(std::make_shared<CRenderable>(wall, (baseFile + "StoneWall.obj").data(), "LightGrey"));
    m_gw->addGameObject(wall);
}

void MainScreen::loadDecorations()
{
    QString basePath = "/course/cs1950u/.archive/2019/student/vulpecula/env/";

    // Power pylon 1
    std::shared_ptr<GameObject> tower1 = std::make_shared<GameObject>("Tower1", m_gw->getNewObjID());
    tower1->addComponent(std::make_shared<CTransform>(tower1, true, glm::vec3(-59.96385, 3.5, -62.57478),
                                                      glm::vec3(0.0f, 1.570795f, 0.0f), glm::vec3(2.0f)));
    tower1->addComponent(std::make_shared<CRenderable>(tower1, basePath + "pylon.obj", "PowerTower"));
    m_gw->addGameObject(tower1);

    // Power pylon 2
    std::shared_ptr<GameObject> tower2 = std::make_shared<GameObject>("Tower2", m_gw->getNewObjID());
    tower2->addComponent(std::make_shared<CTransform>(tower2, true, glm::vec3(0.59753, 0.45378, -55.72558),
                                                      glm::vec3(0.0f, 1.570795f, 0.0f), glm::vec3(2.0f)));
    tower2->addComponent(std::make_shared<CRenderable>(tower2, basePath + "pylon.obj", "PowerTower"));
    auto towerColl = std::make_shared<CollBox>(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(8, 20, 8));
    tower2->addComponent(std::make_shared<CCollider>(tower2, towerColl, false));
    m_gw->addGameObject(tower2);

    // Pylon foundation
    std::shared_ptr<GameObject> foundation = std::make_shared<GameObject>("Foundation", m_gw->getNewObjID());
    auto foundTrans = std::make_shared<CTransform>(foundation, true, glm::vec3(0.59753, -0.1, -55.72558),
                                                   glm::vec3(0), glm::vec3(15.0f, 1.0f, 15.0f));
    foundation->addComponent(foundTrans);
    foundation->addComponent(std::make_shared<CRenderable>(foundation, "cube", "DarkGrey"));
    auto foundColl = std::make_shared<CollBox>(glm::vec3(0), glm::vec3(15.0f, 2.0f, 15.0f));
    auto foundResp = std::make_shared<Standable>(foundTrans.get(), glm::vec3(0));
    foundation->addComponent(std::make_shared<CCollider>(foundation, foundColl, false, foundResp));
    m_gw->addGameObject(foundation);

    // Pylon fence & posts
    std::shared_ptr<GameObject> fence = std::make_shared<GameObject>("ChainFence", m_gw->getNewObjID());
    fence->addComponent(std::make_shared<CTransform>(fence, true, glm::vec3(0.751355, 3.01966, -55.3009)));
    fence->addComponent(std::make_shared<CRenderable>(fence, basePath + "WireFence.obj", "ChainLink"));
    m_gw->addGameObject(fence);
    std::shared_ptr<GameObject> fenceposts = std::make_shared<GameObject>("Fenceposts", m_gw->getNewObjID());
    fenceposts->addComponent(std::make_shared<CTransform>(fenceposts, true, glm::vec3(0.639584, 3.09837, -55.447)));
    fenceposts->addComponent(std::make_shared<CRenderable>(fenceposts, basePath + "WireFencePosts.obj", "LightGrey"));
    m_gw->addGameObject(fenceposts);

    // Power line wire things
    createPrefab(POWER_LINE, glm::vec3(-4.67219, 23.0, -63.88128), glm::vec3(0), glm::vec3(1));
    createPrefab(POWER_LINE, glm::vec3(-4.67219, 23.0, -52.61531), glm::vec3(0), glm::vec3(1));
    createPrefab(POWER_LINE, glm::vec3(-4.67219, 17.26755, -50.16875), glm::vec3(0), glm::vec3(1));
    createPrefab(POWER_LINE, glm::vec3(-4.67219, 17.26755, -66.36208), glm::vec3(0), glm::vec3(1));
    createPrefab(POWER_LINE, glm::vec3(-4.67219, 12.56965, -64.20455), glm::vec3(0), glm::vec3(1));
    createPrefab(POWER_LINE, glm::vec3(-4.67219, 12.56965, -52.38642), glm::vec3(0), glm::vec3(1));

    // River
    std::shared_ptr<GameObject> water = std::make_shared<GameObject>("Water", m_gw->getNewObjID());
    water->addComponent(std::make_shared<CTransform>(water, true, glm::vec3(-39.88062, -7.24381, -11.71469)));
    water->addComponent(std::make_shared<CRenderable>(water, basePath + "water.obj", "Water"));
    m_gw->addGameObject(water);

    std::shared_ptr<GameObject> waterCollider = std::make_shared<GameObject>("WaterCol", m_gw->getNewObjID());
    waterCollider->addComponent(std::make_shared<CTransform>(waterCollider, true, glm::vec3(-39.3268f, -8.33925f, -36.2637f)));
    auto waterColBox = std::make_shared<CollBox>(glm::vec3(0.f), glm::vec3(39.1286f, 2.f, 85.2655f));
    auto waterColResp = std::make_shared<WaterResponse>();
    waterCollider->addComponent(std::make_shared<CCollider>(waterCollider, waterColBox, true, waterColResp));\
    m_gw->addGameObject(waterCollider);

    // Tree prefabs
    createPrefab(PINE_CLUSTER_1, glm::vec3(13.1068, 5.10292, 44.8788), glm::vec3(0), glm::vec3(1.378));
    createPrefab(PINE_CLUSTER_1, glm::vec3(-21.4215, 11.5213, 57.7421), glm::vec3(0, -102, 0), glm::vec3(1.378));
    createPrefab(PINE_CLUSTER_1, glm::vec3(-9.84175, 4.80746, 31.1118), glm::vec3(0, 104, 1.77), glm::vec3(1.698));
    createPrefab(PINE_CLUSTER_1, glm::vec3(-30.2848, 6.64869, -3.73108), glm::vec3(0, -61.4, 0), glm::vec3(1.946));
    createPrefab(PINE_CLUSTER_1, glm::vec3(-35.4438, 7.54247, 16.3926), glm::vec3(0, -209, 0), glm::vec3(2.165));
    createPrefab(PINE_CLUSTER_1, glm::vec3(-52.1693, 5.57712, 48.7226), glm::vec3(0, -54.1, 0), glm::vec3(1.946));
    createPrefab(PINE_CLUSTER_1, glm::vec3(24.2396, 12.4489, -15.3473), glm::vec3(0, 47.1, -2.43), glm::vec3(1.922));

    createPrefab(PINE_CLUSTER_2, glm::vec3(-38.196, 5.14758, 7.87281), glm::vec3(0, -0.386, 0.614), glm::vec3(2.845));
    createPrefab(PINE_CLUSTER_2, glm::vec3(-46.7794, 5.50239, 34.5427), glm::vec3(0, -10.6, -9.36), glm::vec3(2.972));
    createPrefab(PINE_CLUSTER_2, glm::vec3(-56.1566, 4.93349, 59.8012), glm::vec3(0, -0.386, 0.614), glm::vec3(2.845));
    createPrefab(PINE_CLUSTER_2, glm::vec3(4.50326, 7.75222, -23.4455), glm::vec3(0, -19.46, 0.614), glm::vec3(2.845));

    createPrefab(PINE_CLUSTER_3, glm::vec3(29.4384, 3.4635, 42.5876), glm::vec3(0, -37.3, 0), glm::vec3(2.563));
    createPrefab(PINE_CLUSTER_3, glm::vec3(30.0131, 10.7604, -12.2763), glm::vec3(0, -38.4, 1.45), glm::vec3(2.563));

    createPrefab(LONE_PINE, glm::vec3(-16.652, 11.0895, 52.2194), glm::vec3(0, -12, 0), glm::vec3(1.595));
    createPrefab(LONE_PINE, glm::vec3(-24.9716, 9.90567, 63.9324), glm::vec3(0, -27.3, 0), glm::vec3(2.146));

    createPrefab(DEAD_TREES_1, glm::vec3(66.1866, -12.0862, 31.6698), glm::vec3(0, 188, -1.22), glm::vec3(1.295));
    createPrefab(DEAD_TREES_1, glm::vec3(86.3694, -11.9013, 12.1939), glm::vec3(0, 18.8, -9.41), glm::vec3(1));
    createPrefab(DEAD_TREES_1, glm::vec3(84.781, -11.9367, 26.4879), glm::vec3(0, 0, -1.53), glm::vec3(1.113));
    createPrefab(DEAD_TREES_1, glm::vec3(74.3494, -8.62159, 47.7303), glm::vec3(14, 138, 12.6), glm::vec3(1));
    createPrefab(DEAD_TREES_1, glm::vec3(76.9788, 3.40437, 58.9934), glm::vec3(0, -230, 22.6), glm::vec3(1));
}

void MainScreen::loadObjectives()
{
    // Guitar stuff
    std::shared_ptr<GameObject> guitarZone = std::make_shared<GameObject>("GuitarZone", m_gw->getNewObjID());
    guitarZone->addComponent(std::make_shared<CTransform>(guitarZone, true, glm::vec3(-29.74045f, -8.59093f, -29.76656f)));
    auto collCyl = std::make_shared<CollCylinder>(glm::vec3(0.f), 17.0f, 22.0f);
    QStringList guitarSounds = {":/sounds/guitar_01.ogg", ":/sounds/guitar_02.ogg", ":/sounds/guitar_03.ogg",
                                ":/sounds/guitar_04.ogg", ":/sounds/guitar_05.ogg", ":/sounds/guitar_06.ogg",
                                ":/sounds/guitar_07.ogg", ":/sounds/guitar_08.ogg", ":/sounds/guitar_09.ogg"};
    createAudioZone(guitarZone, guitarSounds, collCyl);

    std::shared_ptr<GameObject> guitarStar = std::make_shared<GameObject>("GuitarStar", m_gw->getNewObjID());
    guitarStar->addComponent(std::make_shared<CTransform>(guitarStar, true, glm::vec3(-39.74045f, -3.59093f, -19.76656f),
                                                          glm::vec3(0.f), glm::vec3(0.5f)));
    createStar(guitarStar, ":/sounds/mus_guitar.ogg", guitarZone);

    // Piano stuff
    std::shared_ptr<GameObject> pianoZone = std::make_shared<GameObject>("PianoZone", m_gw->getNewObjID());
    pianoZone->addComponent(std::make_shared<CTransform>(pianoZone, true, glm::vec3(66.5493, -5.79368, 8.51579)));
    //coll = std::make_shared<CollCylinder>(glm::vec3(0.f), 3.0f, 3.0f);
    auto collBox = std::make_shared<CollBox>(glm::vec3(0.f), glm::vec3(56.6197, 22.7298, 122.319));
    QStringList pianoSounds = {":/sounds/piano_01.ogg", ":/sounds/piano_02.ogg", ":/sounds/piano_03.ogg",
                               ":/sounds/piano_04.ogg", ":/sounds/piano_05.ogg", ":/sounds/piano_06.ogg",
                               ":/sounds/piano_07.ogg"};
    createAudioZone(pianoZone, pianoSounds, collBox);

    std::shared_ptr<GameObject> pianoStar = std::make_shared<GameObject>("PianoStar", m_gw->getNewObjID());
    pianoStar->addComponent(std::make_shared<CTransform>(pianoStar, true, glm::vec3(72.46654, -16, -14.88554),
                                                         glm::vec3(0.f, 6.0f, 0.f), glm::vec3(0.5f)));
    createStar(pianoStar, ":/sounds/mus_piano.ogg", pianoZone);

    // Woodwind stuff
    std::shared_ptr<GameObject> woodZone = std::make_shared<GameObject>("WoodZone", m_gw->getNewObjID());
    woodZone->addComponent(std::make_shared<CTransform>(woodZone, true, glm::vec3(4.38416, 0.87211, -49.35240)));
    collCyl = std::make_shared<CollCylinder>(glm::vec3(0.f), 5.0f, 14.0f);
    QStringList woodSounds = {":/sounds/woodwind_01.ogg", ":/sounds/woodwind_02.ogg",
                              ":/sounds/woodwind_03.ogg", ":/sounds/woodwind_04.ogg",
                              ":/sounds/woodwind_05.ogg", ":/sounds/woodwind_06.ogg"};
    createAudioZone(woodZone, woodSounds, collCyl);

    std::shared_ptr<GameObject> woodStar = std::make_shared<GameObject>("WoodStar", m_gw->getNewObjID());
    woodStar->addComponent(std::make_shared<CTransform>(woodStar, true, glm::vec3(4.38416, 1.87211, -51.35240),
                                                        glm::vec3(0.0f), glm::vec3(0.5f)));
    createStar(woodStar, ":/sounds/mus_woodwind.ogg", woodZone);

    // Bell stuff
    std::shared_ptr<GameObject> bellZone = std::make_shared<GameObject>("BellZone", m_gw->getNewObjID());
    bellZone->addComponent(std::make_shared<CTransform>(bellZone, true, glm::vec3(15.24951, -32.1839, 32.34996)));
    collCyl = std::make_shared<CollCylinder>(glm::vec3(0.f), 25.0f, 37.0f);
    QStringList bellSounds = {":/sounds/bell_01.ogg", ":/sounds/bell_02.ogg", ":/sounds/bell_03.ogg",
                              ":/sounds/bell_04.ogg", ":/sounds/bell_05.ogg", ":/sounds/bell_06.ogg",
                              ":/sounds/bell_07.ogg", ":/sounds/bell_08.ogg", ":/sounds/bell_09.ogg"};
    createAudioZone(bellZone, bellSounds, collCyl);

    std::shared_ptr<GameObject> bellStar = std::make_shared<GameObject>("BellStar", m_gw->getNewObjID());
    bellStar->addComponent(std::make_shared<CTransform>(bellStar, true, glm::vec3(34.24951, -30.1839, 5.34996),
                                                        glm::vec3(0.0f), glm::vec3(0.5f)));
    createStar(bellStar, ":/sounds/mus_bell.ogg", bellZone);

    // Mallet stuff
    std::shared_ptr<GameObject> malletZone = std::make_shared<GameObject>("MalletZone", m_gw->getNewObjID());
    malletZone->addComponent(std::make_shared<CTransform>(malletZone, true, glm::vec3(49.7665, 9.95386, -69.8576)));
    //coll = std::make_shared<CollCylinder>(glm::vec3(0.f), 25.0f, 37.0f);
    collBox = std::make_shared<CollBox>(glm::vec3(0.0f), glm::vec3(61.6809, 19.7155, -32.4943));
    QStringList malletSounds = {":/sounds/mallet_01.ogg", ":/sounds/mallet_02.ogg", ":/sounds/mallet_03.ogg",
                                ":/sounds/mallet_04.ogg", ":/sounds/mallet_05.ogg", ":/sounds/mallet_06.ogg",
                                ":/sounds/mallet_07.ogg", ":/sounds/mallet_08.ogg", ":/sounds/mallet_09.ogg",
                                ":/sounds/mallet_10.ogg", ":/sounds/mallet_11.ogg"};
    createAudioZone(malletZone, malletSounds, collBox);

    std::shared_ptr<GameObject> malletStar = std::make_shared<GameObject>("MalletStar", m_gw->getNewObjID());
    malletStar->addComponent(std::make_shared<CTransform>(malletStar, true, glm::vec3(71.6727, 5.96499, -74.1226),
                                                        glm::vec3(0.0f), glm::vec3(0.5f)));
    createStar(malletStar, ":/sounds/mus_mallet.ogg", malletZone);

}

void MainScreen::createAudioZone(std::shared_ptr<GameObject> zoneObj, QStringList files, std::shared_ptr<CollisionVolume> vol)
{
    assert(!files.empty());
    assert(zoneObj != nullptr);
    assert(vol != nullptr);

    auto randomAudio = std::make_shared<RandomAudioSource>(zoneObj, files.first(), "SFX");
    randomAudio->setAmbient(true);
    //randomAudio->setWaitInterval(3.0f, 7.0f);
    randomAudio->setWaitInterval(2.0f, 5.0f);
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

    Graphics *g = Graphics::getGlobalInstance();
    Light l;
    l.type = Light::LIGHT_TYPE::POINT;
    l.pos = glm::vec3(starObj->getComponent<CTransform>()->pos);
    l.color = glm::vec3(0.9f, 0.9f, 0.52549f);
    l.att = glm::vec2(0.15f, 0.01f);
    g->addLight(l);

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

void MainScreen::createPrefab(MainScreen::PrefabType type, glm::vec3 position, glm::vec3 rotationDeg, glm::vec3 scale)
{
    QString basePath("/course/cs1950u/.archive/2019/student/vulpecula/env/");
    glm::vec3 rotation = glm::radians(rotationDeg);

    auto prefab = std::make_shared<GameObject>(QString("PrefabInstance%1").arg(type), m_gw->getNewObjID());
    prefab->addComponent(std::make_shared<CTransform>(prefab, true, position, rotation, scale));

    switch (type) {
    case PINE_CLUSTER_1:
    {
        prefab->addComponent(std::make_shared<CRenderable>(prefab, basePath.append("PineCluster1.obj"), "Pine"));
        glm::vec3 center = glm::rotateY(glm::vec3(0, -6.0 * scale.y, -1.3), rotation.y);
        auto coll = std::make_shared<CollCylinder>(center, 11 * scale.y, 3.4 * scale.x);
        prefab->addComponent(std::make_shared<CCollider>(prefab, coll, false));
        break;
    }
    case PINE_CLUSTER_2:
    {
        prefab->addComponent(std::make_shared<CRenderable>(prefab, basePath.append("PineCluster2.obj"), "Pine"));
        glm::vec3 center = glm::rotateY(glm::vec3(1.2f, -3.0f, -1.3f) * scale, rotation.y);
        auto coll = std::make_shared<CollCylinder>(center, 6 * scale.y, 3 * scale.x);
        prefab->addComponent(std::make_shared<CCollider>(prefab, coll, false));
        break;
    }
    case PINE_CLUSTER_3:
    {
        prefab->addComponent(std::make_shared<CRenderable>(prefab, basePath.append("PineCluster3.obj"), "Pine"));
        break;
    }
    case LONE_PINE:
    {
        prefab->addComponent(std::make_shared<CRenderable>(prefab, basePath.append("LonePine.obj"), "Pine"));
        auto coll = std::make_shared<CollCylinder>(glm::vec3(0, -1.9f * scale.y, 0), 5 * scale.y, 1.1 * scale.x);
        prefab->addComponent(std::make_shared<CCollider>(prefab, coll, false));
        break;
    }
    case DEAD_TREES_1:
    {
        prefab->addComponent(std::make_shared<CRenderable>(prefab, basePath.append("DeadTrees1.obj"), "DeadTrees"));
        break;
    }
    case POWER_LINE:
        prefab->addComponent(std::make_shared<CRenderable>(prefab, basePath.append("wire.obj"), "PowerLine"));
        break;
    }

    m_gw->addGameObject(prefab);
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
    m_parent->getViewPtr()->captureMouse(true);
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
