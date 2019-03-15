#include "GameplayScreen.h"

#include "engine/graphics/Graphics.h"
#include "engine/systems/RenderSystem.h"
#include "engine/systems/FPSCameraSystem.h"
#include "engine/systems/CollisionSystem.h"
#include "engine/components/CTransform.h"
#include "engine/components/CRenderable.h"
#include "engine/components/CInputReceiver.h"
#include "engine/components/volumes/CollCylinder.h"
#include "engine/frame/Application.h"
#include "view.h"

#include "engine/ui/UIObject.h"
#include "engine/ui/UITransform.h"
#include "engine/ui/UIRenderable.h"
#include "engine/ui/UIText.h"
#include "engine/ui/UIDrawer.h"
#include "engine/ui/TextDrawer.h"
#include "engine/ui/UIScriptComp.h"
#include "engine/ui/UIScriptSys.h"
#include "HealthScript.h"

#include "MovementSys.h"
#include "ChunkLoader.h"
#include "NavmeshDrawer.h"
#include "CHealth.h"
#include "CCooldown.h"
#include "CooldownSys.h"
#include "ShootSystem.h"
#include "PlayerCollision.h"
#include "PauseScreen.h"

#include "engine/components/AIComponent.h"
#include "ai/EnemyAISystem.h"
#include "engine/ai/Blackboard.h"

GameplayScreen::GameplayScreen(Application *parent) :
    m_parent(parent)
{
    Graphics *graphics = Graphics::getGlobalInstance();

    graphics->addLight(Light(Light::LIGHT_TYPE::DIRECTIONAL,
                             glm::vec3(0.98f, 0.92f, 0.9f) * 0.7f,
                             glm::vec3(0.1, -0.1, 0.1)));
    graphics->addLight(Light(Light::LIGHT_TYPE::AMBIENT, glm::vec3(0.25f)));

    graphics->addTexture("dungeontex", ":/images/dungeontex.png");
    graphics->addMaterial("dungeon", Material(graphics->getShader("default"), "dungeontex"));

    Material m;
    m.color = glm::vec3(0.9f, 0.9f, 0.09f);
    m.shaderName = "default";
    m.useLighting = false;
    graphics->addMaterial("yellow", m);

    /* m.color = glm::vec3(1.0f, 1.0f, 1.0f);
    graphics->addMaterial("white", m); */

    Material m2;
    m2.color = glm::vec3(1.0f, 0.0f, 0.0f);
    m2.shaderName = "default";
    graphics->addMaterial("red", m2);

    Material m3;
    m3.color = glm::vec3(1.0f, 0.0f, 0.0f);
    m3.shaderName = "default";
    m3.alpha = 0.8f;
    graphics->addMaterial("item", m3);

    Material m4;
    m4.color = glm::vec3(0.1f, 0.67f, 0.93f);
    m4.shaderName = "default";
    graphics->addMaterial("bullet", m4);



    // Initialize the gameworld
    m_gw = std::make_shared<GameWorld>();

    // Collision System - instantiated up here because we use it for both
    // tick AND draw (if we want to debug)
    auto collisionSys = std::make_shared<CollisionSystem>(400);

    // Register draw systems
    m_gw->registerForDraw(std::make_shared<RenderSystem>(200));
    //m_gw->registerForDraw(std::make_shared<NavmeshDrawer>(300)); // Turn on for navmesh draw
    //m_gw->registerForDraw(collisionSys); // Turn on for wireframes

    // Register tick systems
    m_gw->registerForTick(std::make_shared<MovementSys>(100));
    // Later we'll insert the chunk loader here at 150
    m_gw->registerForTick(std::make_shared<EnemyAISystem>(200, m_gw));
    m_gw->registerForTick(std::make_shared<CooldownSys>(250));
    // Later we'll insert the bullet mover/tracker here at 300
    m_gw->registerForTick(collisionSys); // priority 400
    m_gw->registerForTick(std::make_shared<FPSCameraSystem>(500));

    m_gw->registerUISystem(std::make_shared<UIScriptSys>(100));
    m_gw->registerUISystem(std::make_shared<UIDrawer>(200));
    m_gw->registerUISystem(std::make_shared<TextDrawer>(300));

    // Player setup
    std::shared_ptr<GameObject> player = std::make_shared<GameObject>("Player", m_gw->getNewObjID());
    player->addComponent(std::make_shared<CTransform>(player, false, glm::vec3(0.01f, 0.0f, 0.01f)));
    player->addComponent(std::make_shared<CCamera>(player, glm::vec3(0.0f, 0.7f, 0.0f)));
    //player->addComponent(std::make_shared<CRenderable>(player, "cylinder"));
    player->addComponent(std::make_shared<CInputReceiver>(player));
    player->addComponent(std::make_shared<CHealth>(player, 10));
    player->addComponent(std::make_shared<CCooldown>(player, 0.3f));

    // Player collider setup:
    // First we make the volume (a cylinder), then we make the collision responder,
    // then we make the actual collider component and add both to it
    auto vol = std::make_shared<CollCylinder>(glm::vec3(0.0f), 1.0f, 0.2f);
    auto resp = std::make_shared<PlayerCollision>(player, m_gw);
    player->addComponent(std::make_shared<CCollider>(player, vol, false, resp));

    // Add player to gameworld
    m_gw->addGameObject(player);

    // Set up blackboard
    std::shared_ptr<Blackboard> board = std::make_shared<Blackboard>();
    board->addComponent("player_transform", player->getComponent<CTransform>());

    // Add chunk loader system - requires a reference to the board
    m_gw->registerForTick(std::make_shared<ChunkLoader>(150, m_gw, board, 69420));

    // Add the shooting system - requires a reference to the player
    m_gw->registerForTick(std::make_shared<ShootSystem>(250, player, m_gw));


    std::shared_ptr<UIObject> crosshair = std::make_shared<UIObject>("crosshair", m_gw->getNewObjID(), CENTER);
    UITransform t = UITransform(BOT_CENTER, glm::vec2(0.0f, 4.0f), glm::vec2(2,14));
    crosshair->addComponent(std::make_shared<UIRenderable>(crosshair, t, "uiquad", "white"));
    t = UITransform(MID_LEFT, glm::vec2(4.0f, 0.0f), glm::vec2(14,2));
    crosshair->addComponent(std::make_shared<UIRenderable>(crosshair, t, "uiquad", "white"));
    t = UITransform(MID_RIGHT, glm::vec2(-4.0f, 0.0f), glm::vec2(14,2));
    crosshair->addComponent(std::make_shared<UIRenderable>(crosshair, t, "uiquad", "white"));
    t = UITransform(TOP_CENTER, glm::vec2(0.0f, -4.0f), glm::vec2(2,14));
    crosshair->addComponent(std::make_shared<UIRenderable>(crosshair, t, "uiquad", "white"));
    m_gw->addUIObject(crosshair);

    std::shared_ptr<UIObject> text = std::make_shared<UIObject>("text", m_gw->getNewObjID(), BOT_LEFT);
    t = UITransform(BOT_LEFT, glm::vec2(10.0f, 50.0f));
    text->addComponent(std::make_shared<UIText>(text, t, "Health:", 32.0f, "white"));
    m_gw->addUIObject(text);

    graphics->addTexture("red_heart", ":/images/heart.png");
    graphics->addTexture("empty_heart", ":/images/heart_empty.png");
    graphics->addTexture("gold_heart", ":/images/gold.png");

    Material heart;
    heart.shaderName = "default";
    heart.textureName = "red_heart";
    heart.useLighting = false;
    graphics->addMaterial("heart_r", heart);

    heart.textureName = "empty_heart";
    graphics->addMaterial("heart_e", heart);

    heart.textureName = "gold_heart";
    graphics->addMaterial("heart_g", heart);

    Material empty;
    empty.shaderName = "default";
    empty.alpha = 0.0f;
    graphics->addMaterial("hidden", empty);

    auto playerHealth = player->getComponent<CHealth>();

    std::shared_ptr<UIObject> healthDisplay = std::make_shared<UIObject>("Healthyboi", m_gw->getNewObjID(), BOT_LEFT);
    auto scr = std::make_shared<HealthScript>(playerHealth);
    healthDisplay->addComponent(std::make_shared<UIScriptComp>(healthDisplay, scr));

    for (int i = 0; i <= playerHealth->getMaxHealth(); i++) {
        t = UITransform(BOT_LEFT, glm::vec2(10.0f + (40.0f * i), 10.0f), glm::vec2(30));
        auto r = std::make_shared<UIRenderable>(healthDisplay, t, "uiquad", "heart_e");
        healthDisplay->addComponent(r);
        scr->addHeart(r);
    }
    m_gw->addUIObject(healthDisplay);
}

GameplayScreen::~GameplayScreen()
{
}

void GameplayScreen::tick(float seconds)
{
    m_gw->tick(seconds);
}

void GameplayScreen::draw()
{
    m_gw->draw();
}

void GameplayScreen::resize(int w, int h)
{
    m_gw->resize(w, h);
}

void GameplayScreen::onKeyPressed(QKeyEvent *event)
{
    if (event->key() == Qt::Key_P) {
        m_parent->pushScreen(std::make_shared<PauseScreen>(m_parent));
    }
}

void GameplayScreen::onKeyHeld(QKeyEvent *event){}
void GameplayScreen::onKeyReleased(QKeyEvent *event){}
void GameplayScreen::onMousePressed(QMouseEvent *event){}
void GameplayScreen::onMouseReleased(QMouseEvent *event){}
void GameplayScreen::onMouseMoved(QMouseEvent *event, int deltaX, int deltaY){}
void GameplayScreen::onScroll(QWheelEvent *event){}


void GameplayScreen::onFocusGained()
{
    m_parent->getViewPtr()->captureMouse(true);
    m_shouldTick = true;
    m_shouldDraw = true;
}

void GameplayScreen::onFocusLost()
{
    m_shouldTick = false;
    m_shouldDraw = true;
}
