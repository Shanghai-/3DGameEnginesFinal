#include "WinResp.h"

#include "engine/objectManagement/GameWorld.h"

#include "engine/components/CInputReceiver.h"
#include "engine/components/CCollider.h"

#include "engine/systems/AudioSystem.h"

#include "engine/ui/UIObject.h"
#include "engine/ui/UITransform.h"
#include "engine/ui/UIRenderable.h"
#include "engine/ui/UIScriptComp.h"
#include "engine/ui/scripts/FullScreenQuadScript.h"
#include "engine/ui/scripts/TimedFade.h"

#include "vulpecula/scripts/GoToCreds.h"

WinResp::WinResp(GameWorld *gw, Application *app, AudioSystem *audio) :
    m_gw(gw),
    m_app(app),
    m_audio(audio)
{
}

WinResp::~WinResp()
{
}

void WinResp::onCollide(std::shared_ptr<GameObject> other)
{
    if (other->getName() == "Player") {
        other->removeComponent<CInputReceiver>(); // Disable player input
        other->removeComponent<CCollider>(); // No collide no more

        auto fader = std::make_shared<UIObject>("FadeToWhite", m_gw->getNewObjID(), BOT_LEFT);
        auto renderable = std::make_shared<UIRenderable>(fader, UITransform(BOT_LEFT), "uiquad", "WhiteFade");
        fader->addComponent(renderable);

        auto resizeScript = std::make_shared<FullScreenQuadScript>(renderable);
        fader->addComponent(std::make_shared<UIScriptComp>(fader, resizeScript));

        auto fadeScript = std::make_shared<TimedFade>(renderable.get(), 3.5f);
        fader->addComponent(std::make_shared<UIScriptComp>(fader, fadeScript));

        auto goToCreds = std::make_shared<GoToCreds>(m_app, 5.0f);
        fader->addComponent(std::make_shared<UIScriptComp>(fader, goToCreds));

        m_gw->addUIObject(fader);

        m_audio->fadeChannelVolume("Master", 0.0f, 5.0f);
    }
}
