#include "RandomAudioManager.h"

RandomAudioManager::RandomAudioManager(int priority, const std::shared_ptr<AudioSystem> &as) :
    System(priority),
    m_as(as)
{
}

RandomAudioManager::~RandomAudioManager()
{
}

QString RandomAudioManager::getComponentType() const
{
    return typeid(RandomAudioSource).name();
}

inline uint qHash(const std::shared_ptr<RandomAudioSource> &key) {
    return key->getId();
}

void RandomAudioManager::addComponent(const std::shared_ptr<Component> &c)
{
    auto src = std::dynamic_pointer_cast<RandomAudioSource>(c);
    m_sources.insert(src);
    m_as->addComponent(c);
}

void RandomAudioManager::removeComponent(const std::shared_ptr<Component> &c)
{
    m_as->removeComponent(c);

    auto src = std::dynamic_pointer_cast<RandomAudioSource>(c);
    m_sources.remove(src);
}

void RandomAudioManager::tick(float seconds)
{
    QSetIterator<std::shared_ptr<RandomAudioSource>> it(m_sources);
    while (it.hasNext())
    {
        std::shared_ptr<RandomAudioSource> src = it.next();
        if (src->isPaused()) continue; // Ignore paused sources

        if (!src->isPlaying()) {
            if (src->isWaiting()) {
                if (src->incrementWait(seconds)) {
                    src->chooseNewFile();
                    src->playOnce();
                }
            } else {
                src->startNewWait();
            }
        }
    }
}
