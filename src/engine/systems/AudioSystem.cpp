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
    // 1) Allow user to specify channel groups in CAudioComponent
    // 2) Allow user to easily set channel volume (individual and group)
    // 3) Allow user to add/remove DSP units from channel groups (e.g. echo, lowpass)
    //      => User should be able to name DSP units and refer to them by name
    // 4) Add endpoint for getting/setting master volume (for things like settings menus)
    // 5) Specify "I want this sound to loop"
    // 6) Allow sounds to play upon trigger rather than always upon load
    // 7) Look into loading and retaining sounds in memory rather than re-loading (do I even re-load?)
    // 8) Building off of that: delete tempfiles for sounds that are no longer in use
    // 9) Allow user to specify stereo spread (especially/only for ambient sources)

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

    if (!m_files.contains(s->getPath())) {
        auto f = QTemporaryFile::createNativeFile(s->getPath());
        m_files.insert(s->getPath(), f);
    }

    // If the desired sound has not already been loaded, we load it into a tempfile
    // and add it to our loaded sounds map.
    if (s->isAmbient()) {
        errChk(m_sys->createSound(m_files[s->getPath()]->fileName().toStdString().data(), FMOD_3D_HEADRELATIVE, 0, &s->clip));
    } else {
        errChk(m_sys->createSound(m_files[s->getPath()]->fileName().toStdString().data(), FMOD_3D_WORLDRELATIVE, 0, &s->clip));
    }

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

        if (s->channel == nullptr) {
            errChk(m_sys->playSound(s->clip, 0, false, &s->channel));

            if (s->isAmbient()) {
                FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
                s->channel->set3DAttributes(&pos, 0);
            } else {
                glm::vec3 audioLoc = s->getSibling<CTransform>()->pos;
                FMOD_VECTOR pos = { audioLoc.x, audioLoc.y, audioLoc.z };
                s->channel->set3DAttributes(&pos, 0);
            }

            FMOD::DSP *dsp_echo;
            errChk(m_sys->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp_echo));
            errChk(s->channel->addDSP(0, dsp_echo));
        }

        bool playing;
        s->channel->isPlaying(&playing);

        if (!playing) {
            std::cout << "done" << std::endl;
        }
    }

    m_sys->update();
}
