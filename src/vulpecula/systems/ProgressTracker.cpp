#include "ProgressTracker.h"

#include "engine/components/CTransform.h"

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
            // TODO: player won the game! start the endgame sequence
            // (aka put a trigger up on the cliff that, when they walk into it, pans the camera up to the sky)
        }
    }
}

void ProgressTracker::tick(float seconds)
{
    // TODO: Keep star counter UI element up to date
}
