#include "MainScreen.h"

#include <iostream>

#include "engine/graphics/Graphics.h"
#include "engine/objectManagement/GameWorld.h"
#include "engine/systems/RenderSystem.h"
#include "engine/systems/FPSCameraSystem.h"
#include "engine/systems/CollisionSystem.h"
#include "engine/systems/RaycastSystem.h"
#include "CustomSystems/PhysicsSystem.h"
#include "CustomSystems/InputSystem.h"
#include "engine/components/CCamera.h"
#include "engine/components/CCollider.h"
#include "engine/components/CRenderable.h"
#include "engine/components/CTransform.h"
#include "engine/components/volumes/CollCylinder.h"
#include "engine/components/volumes/CollBox.h"
#include "engine/components/volumes/CollSphere.h"
#include "engine/components/CInputReceiver.h"
#include "CustomComponents/CPhysics.h"
#include "engine/gen/SpacePartMap.h"
#include "engine/systems/AudioSystem.h"

#include "engine/frame/Application.h"
#include "view.h"

#define FLOOR_SCALE 50.0f

MainScreen::MainScreen(Application *parent) :
    m_parent(parent)
{
    Graphics *graphics = Graphics::getGlobalInstance();
    graphics->addMaterial("floorMaterial", Material("default", "grass", glm::vec2(10.0f, 10.0f)));

    Material m = Material();
    m.color = glm::vec3(0.0f, 1.0f, 0.0f);
    graphics->addMaterial("hit", m);

    Material other_m = Material();
    other_m.color = glm::vec3(1.0f, 0.0f, 0.0f);
    graphics->addMaterial("not_hit", other_m);

    // Add a directional light and an ambient light to the scene
    graphics->addLight(Light(Light::LIGHT_TYPE::DIRECTIONAL,
                               glm::vec3(0.98f, 0.92f, 0.9f) * 0.9f,
                               glm::vec3(0.1, -0.5, 0.1)));
    graphics->addLight(Light(Light::LIGHT_TYPE::AMBIENT,
                               glm::vec3(0.1f, 0.0, 0.2f)));
    graphics->setClearColor(glm::vec3(0.05f, 0.3f, 0.5f));


    // TODO: encapsulate this in a basic_gameworld_factory class in util
    m_gw = std::make_unique<GameWorld>();
    m_gw->registerForDraw(std::make_shared<RenderSystem>(500));
    m_gw->registerForTick(std::make_shared<InputSystem>(200));
    m_gw->registerForTick(std::make_shared<PhysicsSystem>(400));
    m_gw->registerForTick(std::make_shared<CollisionSystem>(500));
    m_gw->registerForTick(std::make_shared<FPSCameraSystem>(600));
    m_gw->registerForTick(std::make_shared<AudioSystem>(800));

    // TODO: encapsulate this in a player_factory class
    std::shared_ptr<GameObject> player = std::make_shared<GameObject>("Player", m_gw->getNewObjID());
    player->addComponent(std::make_shared<CTransform>(player, false, glm::vec3(0.0f, 1.0f, 0.0f)));
    player->addComponent(std::make_shared<CPhysics>(player));
    player->addComponent(std::make_shared<CRenderable>(player, "cube"));
    player->addComponent(std::make_shared<CCamera>(player, glm::vec3(0.0f, 0.5f, 0.0f)));
    auto coll = std::make_shared<CollCylinder>(glm::vec3(0.0f), 1.0f, 0.5f);
    //auto coll = std::make_shared<CollBox>(glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    //auto coll = std::make_shared<CollSphere>(glm::vec3(0.0f, 0.5f, 0.0f), 0.5f);
    player->addComponent(std::make_shared<CCollider>(player, coll, false));
    player->addComponent(std::make_shared<CInputReceiver>(player));
    m_gw->addGameObject(player);

    // Floor - an immobile thing
    std::shared_ptr<GameObject> floor = std::make_shared<GameObject>("Floor", m_gw->getNewObjID());
    floor->addComponent(std::make_shared<CTransform>(floor, true, glm::vec3(0.0f),
                                      glm::vec3(0.f), glm::vec3(FLOOR_SCALE, 1.0f, FLOOR_SCALE)));
    floor->addComponent(std::make_shared<CRenderable>(floor, "quad", "floorMaterial"));
    m_gw->addGameObject(floor);

    // Movable cylinder
    std::shared_ptr<GameObject> collider = std::make_shared<GameObject>("Collider", m_gw->getNewObjID());
    collider->addComponent(std::make_shared<CTransform>(collider, false,
                                          glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(0.f), glm::vec3(0.8f, 2.0f, 0.8f)));
    collider->addComponent(std::make_shared<CRenderable>(collider, "cylinder"));
    collider->addComponent(std::make_shared<CCollider>(collider,
        std::make_shared<CollCylinder>(glm::vec3(0.0f), 2.0f, 0.4f), false));
    m_gw->addGameObject(collider);

    // Static cylinder
    std::shared_ptr<GameObject> squatboi = std::make_shared<GameObject>("Static", m_gw->getNewObjID());
    squatboi->addComponent(std::make_shared<CTransform>(squatboi, true,
                                          glm::vec3(2.0f, 0.0f, -3.0f), glm::vec3(0.f), glm::vec3(2.0f, 0.3f, 2.0f)));
    squatboi->addComponent(std::make_shared<CRenderable>(squatboi, "cylinder"));
    squatboi->addComponent(std::make_shared<CCollider>(squatboi,
        std::make_shared<CollCylinder>(glm::vec3(0.0f), 0.3f, 1.0f), false));
    m_gw->addGameObject(squatboi);

    // Moving box
    std::shared_ptr<GameObject> boxe = std::make_shared<GameObject>("Box1", m_gw->getNewObjID());
    boxe->addComponent(std::make_shared<CTransform>(boxe, false, glm::vec3(-3.0f, 0.0f, 0.0f)));
    boxe->addComponent(std::make_shared<CRenderable>(boxe, "cube"));
    boxe->addComponent(std::make_shared<CCollider>(boxe,
        std::make_shared<CollBox>(glm::vec3(0.0f), glm::vec3(1.0f)), false));
    m_gw->addGameObject(boxe);

    std::shared_ptr<GameObject> test = std::make_shared<GameObject>("heart", m_gw->getNewObjID());
    test->addComponent(std::make_shared<CTransform>(test, true, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.005f)));
    test->addComponent(std::make_shared<CRenderable>(test, ":/models/Love.obj", "not_hit"));
    m_gw->addGameObject(test);
}

MainScreen::~MainScreen()
{
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

void MainScreen::onKeyHeld(QKeyEvent *event) {}
void MainScreen::onKeyPressed(QKeyEvent *event) {}
void MainScreen::onKeyReleased(QKeyEvent *event) {}
void MainScreen::onMousePressed(QMouseEvent *event) {}
void MainScreen::onMouseReleased(QMouseEvent *event) {}
void MainScreen::onScroll(QWheelEvent *event) {}
void MainScreen::onMouseMoved(QMouseEvent *event, int deltaX, int deltaY) {}


void MainScreen::onFocusGained()
{
    m_parent->getViewPtr()->captureMouse(true);
}

void MainScreen::onFocusLost()
{
}
