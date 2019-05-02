#include "AudioSystem.h"

#include "fmod/fmod_errors.h"
#include "engine/components/CTransform.h"
#include "glm/gtx/string_cast.hpp"

#include <iostream>
#include <QTemporaryFile>

/**
 * @brief AudioSystem::errChk can be wrapped around FMOD library calls to check
 *        if they have encountered an error. If they do, it prints the error
 *        and causes the program to exit.
 * @param res - the result to check
 */
void AudioSystem::errChk(FMOD_RESULT res)
{
    if (res != FMOD_OK) {
        std::cerr << "FMOD error! (" << res << "): " << FMOD_ErrorString(res) << std::endl;
        exit(res);
    }
}

AudioSystem::AudioSystem(int priority, std::shared_ptr<GameObject> listener) :
    System(priority),
    m_oldX(0.0f),
    m_oldY(0.0f),
    m_oldZ(0.0f)
{
    // Create and initialize the FMOD system object
    errChk(FMOD::System_Create(&m_sys));
    errChk(m_sys->init(NUM_CHANNELS, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, nullptr));

    // TODO:
    // TESTING Allow user to add/remove DSP units from channel groups (e.g. echo, lowpass)
    //      => User should be able to name DSP units and refer to them by name
    // 7) Look into loading and retaining sounds in memory rather than re-loading (do I even re-load?)
    // 8) Building off of that: delete tempfiles for sounds that are no longer in use

    // DONE:
    // Add endpoint for getting/setting master volume (for things like settings menus)
    // Use CAudioComponent's channel group no. to put it into the given channelgroup
    // Looping
    // Allow user to easily set channel volume (individual and group)
    // Allow sounds to play upon trigger rather than always upon load
    // Allow user to specify stereo spread (especially/only for ambient sources)

    FMOD::ChannelGroup *master;
    errChk(m_sys->getMasterChannelGroup(&master));
    m_channels.insert("Master", master);

    m_listener = listener->getComponent<CCamera>();
}

AudioSystem::~AudioSystem()
{
    // TODO: Check this for leaks
    FMOD::Channel *toDelete;
    FMOD_RESULT res;
    for (int i = 0; i < NUM_CHANNELS; i++) {
        res = m_sys->getChannel(i, &toDelete);

        if (res == FMOD_OK && toDelete) {
            toDelete->stop();
        }
    }

    m_files.empty();

    m_sys->release();
}

QString AudioSystem::getComponentType() const
{
    return typeid(CAudioSource).name();
}

inline uint qHash(const std::shared_ptr<CAudioSource> &key) {
    return key->getId();
}

void AudioSystem::addComponent(const std::shared_ptr<Component> &c)
{
    auto s = std::dynamic_pointer_cast<CAudioSource>(c);
    loadSound(s);
    m_sources.insert(s);
}

void AudioSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    auto s = std::dynamic_pointer_cast<CAudioSource>(c);
    m_sources.remove(s);
}

void AudioSystem::tick(float seconds)
{
    auto graphicsCam = m_listener->getCameraObject();

    glm::vec3 camPos = graphicsCam->getEye();
    FMOD_VECTOR pos = { camPos.x, camPos.y, camPos.z };

    glm::vec3 camForward = graphicsCam->getLook();
    FMOD_VECTOR forward = { camForward.x, camForward.y, camForward.z };

//    std::cout << "look: " << glm::to_string(graphicsCam->getLook()) << std::endl;
//    std::cout << "left: " << glm::to_string(graphicsCam->getLookPerp()) << std::endl;

    glm::vec3 camUp = glm::cross(camForward, graphicsCam->getLookPerp());
    FMOD_VECTOR up = { camUp.x, camUp.y, camUp.z };

//    std::cout << "up: " << glm::to_string(camUp) << std::endl;

    FMOD_VECTOR vel = { (camPos.x - m_oldX) / seconds,
                        (camPos.y - m_oldY) / seconds,
                        (camPos.z - m_oldZ) / seconds };
    m_oldX = camPos.x;
    m_oldY = camPos.y;
    m_oldZ = camPos.z;

    m_sys->set3DListenerAttributes(0, &pos, &vel, &forward, &up);

    QSetIterator<std::shared_ptr<CAudioSource>> it(m_sources);
    while (it.hasNext())
    {
        std::shared_ptr<CAudioSource> s = it.next();

        if (s->fileChanged()) {
            if (s->channel != nullptr) s->channel->stop();
            s->channel = nullptr;
            loadSound(s);
            s->resetFile();
        }

        if (s->isPlaying()) {
            if (s->channel == nullptr) {
                errChk(m_sys->playSound(s->clip, 0, false, &s->channel));

                // If the sound is ambient, we place it inside the player's head
                if (s->isAmbient()) {
                    FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
                    s->channel->set3DAttributes(&pos, 0);

                } else {
                    // Otherwise, we place it in world-space according to its parent gameobject's transform
                    glm::vec3 audioLoc = s->getSibling<CTransform>()->pos;
                    FMOD_VECTOR pos = { audioLoc.x, audioLoc.y, audioLoc.z };
                    s->channel->set3DAttributes(&pos, 0);
                }

                // If the sound is supposed to loop, we make its channel loop
                if (s->shouldLoop()) {
                    s->channel->setMode(FMOD_LOOP_NORMAL);
                } else {
                    s->channel->setMode(FMOD_LOOP_OFF);
                }

                // Apply the requested stereo spread
                s->channel->set3DSpread(s->getStereoSpread());

                QString channelGroupName = s->getChannelGroup();
                std::cout << qPrintable(channelGroupName) << std::endl;
                FMOD::ChannelGroup *cg = m_channels.value(channelGroupName);

                if (cg == nullptr) {
                    std::cout << "crearing..." << std::endl;
                    m_sys->createChannelGroup(qPrintable(channelGroupName), &cg);
                    m_channels.insert(channelGroupName, cg);
                }

                // Add the sound to the group
                s->channel->setChannelGroup(cg);
            }

            // Set volume
            s->channel->setVolume(s->getVolume());

            // If the sound is muted, mute the channel
            if (s->isMuted()) {
                s->channel->setMute(true);
            } else {
                s->channel->setMute(false);
            }

            // Since the sound is playing, not paused, the channel shouldn't be paused
            s->channel->setPaused(false);

            // If the sound says it's playing but the channel says it's not, then that means
            // the sound has finished and is no longer actually playing. So we tell the sound
            // that it is stopped and reset its channel to null.
            bool playing;
            s->channel->isPlaying(&playing);

            if (!playing) {
                s->stop();
                s->channel = nullptr;
            }

        } else {
            // If the sound is stopped, we tell FMOD to stop playing and clear the channel
            if (s->isStopped() && s->channel != nullptr) {
                bool playing;
                errChk(s->channel->isPlaying(&playing));

                if (playing) {
                    errChk(s->channel->stop());
                }

                s->channel = nullptr;
            }

            // If the sound is PAUSED (not stopped), we just pause the channel
            if (s->isPaused() && s->channel != nullptr) {
                errChk(s->channel->setPaused(true));
            }
        }
    }

    m_sys->update();
}

/**********************************
 * CHANNEL MANIPULATION FUNCTIONS *
 **********************************/


void AudioSystem::createChannel(const QString &name)
{
    if (m_channels.contains(name)) return; // Already exists

    FMOD::ChannelGroup *cg;
    errChk(m_sys->createChannelGroup(qPrintable(name), &cg));
    m_channels.insert(name, cg);
}

void AudioSystem::deleteChannel(const QString &name)
{
    if (name == "Master") return; // Cannot delete master, it's reserved

    FMOD::ChannelGroup *cg = m_channels.take(name);
    cg->stop();
    cg->release();
}

FMOD::ChannelGroup *AudioSystem::getChannel(const QString &name)
{
    return m_channels.value(name);
}

void AudioSystem::setChannelChild(const QString &parent, const QString &child)
{
    FMOD::ChannelGroup *p = m_channels.value(parent);
    FMOD::ChannelGroup *c = m_channels.value(child);

    if (p == nullptr || c == nullptr) {
        return; // TODO: Throw an exception
    }

    errChk(p->addGroup(c));
}

void AudioSystem::setChannelVolume(const QString &name, const float &volume)
{
    FMOD::ChannelGroup *cg = m_channels.value(name);
    if (cg != nullptr) cg->setVolume(volume);
}

void AudioSystem::setChannelMuted(const QString &name, const bool &isMuted)
{
    FMOD::ChannelGroup *cg = m_channels.value(name);
    if (cg != nullptr) cg->setMute(isMuted);
}

void AudioSystem::fadeChannelVolume(const QString &name, const float &targetVolume, const float &timeInSeconds)
{
    FMOD::ChannelGroup *cg = m_channels.value(name);
    if (cg != nullptr) {
        unsigned long long clock;
        cg->getDSPClock(&clock, nullptr);
        cg->setFadePointRamp(clock + (timeInSeconds * 44100.0f), targetVolume);
    }
}


/*******************************
 * EFFECT MANAGEMENT FUNCTIONS *
 *******************************/

FMOD::DSP *AudioSystem::createEffect(FMOD_DSP_TYPE type)
{
    FMOD::DSP *dsp;
    errChk(m_sys->createDSPByType(type, &dsp));

    return dsp;
}

void AudioSystem::addEffectAtStart(FMOD::DSP *effect, QString effectName, QString channelName, bool pre_fader)
{
    FMOD::ChannelGroup *cg = m_channels.value(channelName);
    if (cg == nullptr) return;

    if (pre_fader) {
        errChk(cg->addDSP(FMOD_CHANNELCONTROL_DSP_HEAD, effect));
    } else {
        errChk(cg->addDSP(FMOD_CHANNELCONTROL_DSP_FADER + 1, effect));
    }

    m_effects.insert(channelName.append(effectName), effect);
}

void AudioSystem::addEffectAtEnd(FMOD::DSP *effect, QString effectName, QString channelName, bool pre_fader)
{
    FMOD::ChannelGroup *cg = m_channels.value(channelName);
    if (cg == nullptr) return;

    if (pre_fader) {
        errChk(cg->addDSP(FMOD_CHANNELCONTROL_DSP_FADER - 1, effect));
    } else {
        errChk(cg->addDSP(FMOD_CHANNELCONTROL_DSP_TAIL, effect));
    }

    m_effects.insert(channelName.append(effectName), effect);
}

void AudioSystem::addEffectAtIndex(FMOD::DSP *effect, QString effectName, QString channelName, bool pre_fader, int index)
{
    FMOD::ChannelGroup *cg = m_channels.value(channelName);
    if (cg == nullptr) return;

    if (pre_fader) {
        errChk(cg->addDSP(index, effect));
    } else {
        errChk(cg->addDSP(FMOD_CHANNELCONTROL_DSP_FADER + index, effect));
    }

    m_effects.insert(channelName.append(effectName), effect);
}

FMOD::DSP *AudioSystem::getEffect(QString effectName, QString channelName)
{
    return m_effects.value(channelName.append(effectName));
}

void AudioSystem::removeEffect(QString effectName, QString channelName)
{
    FMOD::ChannelGroup *cg = m_channels.value(channelName);
    if (cg == nullptr) return;

    FMOD::DSP *effect = m_effects.take(channelName.append(effectName));

    errChk(cg->removeDSP(effect));
    effect->release();
}

/*****************
 * SOUND LOADING *
 *****************/

void AudioSystem::loadSound(std::shared_ptr<CAudioSource> s)
{
    // If the desired sound has not already been loaded, we load it into a tempfile
    // and add it to our loaded sounds map.
    if (!m_files.contains(s->getPath())) {
        auto f = QTemporaryFile::createNativeFile(s->getPath());
        m_files.insert(s->getPath(), f);
    }

    QString fname = m_files[s->getPath()]->fileName();
    if (s->isAmbient()) {
        errChk(m_sys->createSound(qPrintable(fname), FMOD_3D_HEADRELATIVE, 0, &s->clip));
    } else {
        errChk(m_sys->createSound(qPrintable(fname), FMOD_3D_WORLDRELATIVE, 0, &s->clip));
    }

    // Set loop points at the start and end of the clip
    // TODO: This defaults to start and end, which loops the whole clip.
    // We can play with start and end for funsies. Maybe make API calls for it?
    /* unsigned int length;
    s->clip->getLength(&length, FMOD_TIMEUNIT_MS);
    s->clip->setLoopPoints(400, FMOD_TIMEUNIT_MS, 1000, FMOD_TIMEUNIT_MS);*/

    if (s->shouldLoop()) {
        // Prepare sound for looping
        s->clip->setMode(FMOD_LOOP_NORMAL);
    } else {
        // Specify no looping
        s->clip->setMode(FMOD_LOOP_OFF);
    }
}
