#include "CreditsScreen.h"

#include "engine/frame/Application.h"
#include "view.h"

#include "engine/ui/TextDrawer.h"
#include "engine/ui/UIText.h"
#include "engine/ui/UIObject.h"
#include "engine/ui/UITransform.h"
#include "engine/ui/UIRenderable.h"
#include "engine/ui/UIDrawer.h"
#include "engine/ui/UIScriptSys.h"
#include "engine/ui/UIScriptComp.h"
#include "engine/ui/scripts/FullScreenQuadScript.h"
#include "engine/ui/scripts/TimedFade.h"

#include "engine/graphics/Graphics.h"

CreditsScreen::CreditsScreen(Application *parent) :
    m_parent(parent)
{
    m_gw = std::make_unique<GameWorld>();

    m_gw->registerUISystem(std::make_shared<UIScriptSys>(50));
    m_gw->registerUISystem(std::make_shared<UIDrawer>(75));
    m_gw->registerUISystem(std::make_shared<TextDrawer>(100));

    Graphics *g = Graphics::getGlobalInstance();

    Material black;
    black.useLighting = false;
    black.color = glm::vec3(0.05f);
    black.alpha = 0.0f;
    g->addMaterial("UIBlack", black);

    Material darkG;
    darkG.useLighting = false;
    darkG.color = glm::vec3(0.3f);
    darkG.alpha = 0.0f;
    g->addMaterial("UIDarkG", darkG);
    g->addMaterial("UIDarkGTwo", darkG);

    std::shared_ptr<UIObject> background = std::make_shared<UIObject>("Background", m_gw->getNewObjID(), BOT_LEFT);
    auto renderable = std::make_shared<UIRenderable>(background, UITransform(BOT_LEFT), "uiquad", "UIWhite");
    background->addComponent(renderable);
    auto resizeScript = std::make_shared<FullScreenQuadScript>(renderable);
    background->addComponent(std::make_shared<UIScriptComp>(background, resizeScript));
    m_gw->addUIObject(background);

    float fontSize = 72.0f;

    std::shared_ptr<UIObject> header = std::make_shared<UIObject>("Header", m_gw->getNewObjID(), CENTER);
    UITransform t = UITransform(TOP_CENTER, glm::vec2(0, 180));
    auto headerText = std::make_shared<UIText>(header, t, "A game by:", fontSize, "UIBlack", "LatoThin");
    header->addComponent(headerText);
    auto fadeScript = std::make_shared<TimedFade>(headerText.get(), 1.5f);
    header->addComponent(std::make_shared<UIScriptComp>(header, fadeScript));
    m_gw->addUIObject(header);

    std::shared_ptr<UIObject> name1 = std::make_shared<UIObject>("Brendan", m_gw->getNewObjID(), CENTER);
    t = UITransform(BOT_CENTER, glm::vec2(0, 40));
    auto name1Text = std::make_shared<UIText>(name1, t, "Brendan Walsh", fontSize, "UIBlack", "LatoThin");
    name1->addComponent(name1Text);
    m_gw->addUIObject(name1);

    std::shared_ptr<UIObject> name2 = std::make_shared<UIObject>("Dain", m_gw->getNewObjID(), CENTER);
    t = UITransform(BOT_CENTER, glm::vec2(0, -20));
    auto name2Text = std::make_shared<UIText>(name2, t, "Dain Woods", fontSize, "UIBlack", "LatoThin");
    name2->addComponent(name2Text);
    m_gw->addUIObject(name2);

    std::shared_ptr<UIObject> name3 = std::make_shared<UIObject>("Brandon", m_gw->getNewObjID(), CENTER);
    t = UITransform(TOP_CENTER, glm::vec2(0, -40));
    auto name3Text = std::make_shared<UIText>(name3, t, "Brandon Li", fontSize, "UIBlack", "LatoThin");
    name3->addComponent(name3Text);
    m_gw->addUIObject(name3);

    std::shared_ptr<UIObject> artCreds = std::make_shared<UIObject>("ArtStuff", m_gw->getNewObjID(), BOT_CENTER);
    t = UITransform(BOT_CENTER, glm::vec2(0, 420));
    QString txt = "Title art by Odin Vatne";
    artCreds->addComponent(std::make_shared<UIText>(artCreds, t, txt, 42.0f, "UIDarkGTwo", "LatoThin"));
    t = UITransform(BOT_CENTER, glm::vec2(0, 380));
    txt = "UI art by Beatriz Mora";
    auto artText = std::make_shared<UIText>(artCreds, t, txt, 42.0f, "UIDarkGTwo", "LatoThin");
    artCreds->addComponent(artText);
    auto artFade = std::make_shared<TimedFade>(artText.get(), 1.5f, 2.5f);
    artCreds->addComponent(std::make_shared<UIScriptComp>(artCreds, artFade));
    m_gw->addUIObject(artCreds);

    std::shared_ptr<UIObject> soundCreds = std::make_shared<UIObject>("SoundStuff", m_gw->getNewObjID(), BOT_LEFT);
    t = UITransform(BOT_LEFT, glm::vec2(20, 160));
    txt = "Wind chime sounds by giddster (freesound.org/people/giddster),";
    auto soundText = std::make_shared<UIText>(soundCreds, t, txt, 38.0f, "UIDarkG", "LatoThin");
    soundCreds->addComponent(soundText);
    t = UITransform(BOT_LEFT, glm::vec2(340, 125));
    txt = "acclivity (freesound.org/people/acclivity),";
    soundText = std::make_shared<UIText>(soundCreds, t, txt, 38.0f, "UIDarkG", "LatoThin");
    soundCreds->addComponent(soundText);
    t = UITransform(BOT_LEFT, glm::vec2(340, 90));
    txt = "and OPMartin (freesound.org/people/OPMartin).";
    soundText = std::make_shared<UIText>(soundCreds, t, txt, 38.0f, "UIDarkG", "LatoThin");
    soundCreds->addComponent(soundText);
    t = UITransform(BOT_LEFT, glm::vec2(20, 55));
    txt = "Wind sound by Bosk1 (freesound.org/people/Bosk1).";
    soundText = std::make_shared<UIText>(soundCreds, t, txt, 38.0f, "UIDarkG", "LatoThin");
    soundCreds->addComponent(soundText);
    t = UITransform(BOT_LEFT, glm::vec2(20, 20));
    txt = "All sounds licensed under Creative Commons with Attribution.";
    soundText = std::make_shared<UIText>(soundCreds, t, txt, 38.0f, "UIDarkG", "LatoThin");
    soundCreds->addComponent(soundText);
    auto otherFadeScript = std::make_shared<TimedFade>(soundText.get(), 1.5f, 6.0f);
    soundCreds->addComponent(std::make_shared<UIScriptComp>(soundCreds, otherFadeScript));
    m_gw->addUIObject(soundCreds);

    std::shared_ptr<UIObject> quitText = std::make_shared<UIObject>("QuitText", m_gw->getNewObjID(), TOP_LEFT);
    t = UITransform(TOP_LEFT, glm::vec2(20, -20));
    txt = "Press [Esc] to quit.";
    quitText->addComponent(std::make_shared<UIText>(quitText, t, txt, 38.0f, "UIDarkG", "LatoThin"));
    m_gw->addUIObject(quitText);
}

CreditsScreen::~CreditsScreen()
{
}

void CreditsScreen::tick(float seconds)
{
    m_gw->tick(seconds);
}

void CreditsScreen::draw()
{
    m_gw->draw();
}

void CreditsScreen::resize(int w, int h)
{
    m_gw->resize(w, h);
}

void CreditsScreen::onFocusGained()
{
    m_parent->getViewPtr()->captureMouse(false);
}

void CreditsScreen::onFocusLost()
{
}
