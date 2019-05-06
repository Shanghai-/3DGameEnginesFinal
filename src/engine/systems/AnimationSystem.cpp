#include "AnimationSystem.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "engine/graphics/Graphics.h"
#include "engine/graphics/ShaderAttribLocations.h"
#include "engine/components/CTransform.h"

AnimationSystem::AnimationSystem(int priority) :
    System(priority)
{
    m_graphics = Graphics::getGlobalInstance();
    m_graphics->addShader("Animated", ":/shaders/animated.vert", ":/shaders/shader.frag");
}

AnimationSystem::~AnimationSystem()
{
}

QString AnimationSystem::getComponentType() const
{
    return typeid(CAnimatedMesh).name();
}

inline uint qHash(const std::shared_ptr<CAnimatedMesh> &key) {
    return key->getId();
}

void AnimationSystem::addComponent(const std::shared_ptr<Component> &c)
{
    auto mesh = std::dynamic_pointer_cast<CAnimatedMesh>(c);
    m_meshes.insert(mesh);
}

void AnimationSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    auto mesh = std::dynamic_pointer_cast<CAnimatedMesh>(c);
    m_meshes.remove(mesh);
}

void AnimationSystem::tick(float seconds)
{
    // Iterate over all the meshes
    QSetIterator<std::shared_ptr<CAnimatedMesh>> it(m_meshes);
    while (it.hasNext()) {
        std::shared_ptr<CAnimatedMesh> m = it.next();

        // We need to get the mesh, armature, and animation to figure out all the deforms
        QString curr = m->getCurrentAnimation();
        RiggedMesh *rm = m->getMesh();
        Armature skeleton = rm->getArmature();

        // If the component has an animation playing, we do this stuff
        if (curr != "None") {
            Animation a = rm->getAnimation(curr);

            // Convert the time the animation has been running from seconds to animation ticks
            // These do not necessarily match the tick rate of the engine, which is 60 TPS.
            float timeInTicks = m->addSeconds((seconds * 1.5)) * a.getTicksPerSec();

            // Convert the animation running time (in ticks) to a frame in the animation.
            // If the animation loops, that means we take the time mod the duration, so it starts over if we go past the end
            // If the animation doesn't loop, we take the min of the duration and the time, so that it stays on the last frame
            float curFrame = (a.shouldLoop()) ? std::fmod(timeInTicks, a.getDuration()) : std::min(timeInTicks, a.getDuration());

            // Get the set of root bones (usually there's just one) from the skeleton and start updating from the root
            std::vector<Armature::Bone *> roots = skeleton.getRoots();
            for (uint i = 0; i < roots.size(); i++) {
                // the third argument here is the parent transform. Logically it SHOULD be the identity,
                // because the root has no parent, but in practice setting it to the root's offset flips
                // the model upright (whereas with identity, it loads on its side).
                updateBone(a, roots[i], roots[i]->offset, skeleton.getGlobalInverse(), curFrame);
            }
        } else {
            // If there's no animation playing, just reset to the rest pose
            skeleton.goToRestPose();
        }

        // At this point, the armature is fully posed. We will pass the transforms to the shader
        // during drawing.
    }
}

void AnimationSystem::draw()
{
    m_graphics->setShader("Animated");
    std::shared_ptr<Shader> s = m_graphics->getActiveShader();

    QSetIterator<std::shared_ptr<CAnimatedMesh>> it(m_meshes);
    while (it.hasNext()) {
        std::shared_ptr<CAnimatedMesh> m = it.next();
        RiggedMesh *rm = m->getMesh();
        Armature skeleton = rm->getArmature();

        auto trans = m->getSibling<CTransform>();

        m_graphics->clearTransform();
        m_graphics->translate(trans->pos);
        m_graphics->rotate(trans->rot.x, glm::vec3(1,0,0));
        m_graphics->rotate(trans->rot.y, glm::vec3(0,1,0));
        m_graphics->rotate(trans->rot.z, glm::vec3(0,0,1));
        m_graphics->scale(trans->scale);

        for (uint i = 0; i < skeleton.getNumBones(); i++) {
            s->setUniformArrayByIndex("boneTrans", skeleton.getBone(i)->finalTransform, i);
        }

        if (!m->useModelMaterials()) {
            m_graphics->setMaterial(m->getMaterial());
            assert(m_graphics->getActiveShader() == s);
        }

        rm->draw();
    }
}

void AnimationSystem::updateBone(Animation anim, Armature::Bone *bone,
                                 glm::mat4 parentTransform, glm::mat4 globalInverse,
                                 float curFrame)
{
    glm::mat4 transform = bone->transform;

    // Check if current bone has any keys in current animation
    if (anim.hasKeys(bone->name)) {
        Animation::Channel c = anim.getKeys(bone->name);

        glm::mat4 scale = glm::scale(interpolateScale(c, curFrame));
        glm::mat4 rotate = glm::toMat4(interpolateRotation(c, curFrame));
        glm::mat4 translate = glm::translate(interpolatePosition(c, curFrame));

        transform = translate * rotate * scale;
    } else {
        std::cout << "No keyframes for " << qPrintable(bone->name) << std::endl;
    }

    glm::mat4 globalTransform = parentTransform * transform;

    bone->finalTransform = globalInverse * globalTransform * bone->offset;
    //bone->finalTransform = globalTransform * bone->offset;

    for (uint i = 0; i < bone->children.size(); i++) {
        updateBone(anim, bone->children[i], globalTransform, globalInverse, curFrame);
    }
}

glm::vec3 AnimationSystem::interpolateScale(Animation::Channel c, float curFrame)
{
    // If there's only one keyframe, we can't interpolate. Just return instead.
    if (c.scales.size() == 1) return c.scales[0];
    if (curFrame >= c.scale_frames.back()) return c.scales.back();

    uint lastKeyframe, nextKeyframe;
    for (lastKeyframe = 0; lastKeyframe < c.scale_frames.size() - 1; lastKeyframe++) {
        if (curFrame < c.scale_frames[lastKeyframe + 1]) break;
    }
    nextKeyframe = lastKeyframe + 1;

    float keyframeTimeDiff = c.scale_frames[nextKeyframe] - c.scale_frames[lastKeyframe];
    float factor = (curFrame - c.scale_frames[lastKeyframe]) / keyframeTimeDiff;
    factor = glm::clamp(factor, 0.0f, 1.0f);

    glm::vec3 start = c.scales[lastKeyframe];
    glm::vec3 end = c.scales[nextKeyframe];

    return glm::mix(start, end, factor);
}

glm::quat AnimationSystem::interpolateRotation(Animation::Channel c, float curFrame)
{
    if (c.rotations.size() == 1) return c.rotations[0];
    if (curFrame >= c.rot_frames.back()) return c.rotations.back();

    uint lastKeyframe, nextKeyframe;
    for (lastKeyframe = 0; lastKeyframe < c.rot_frames.size() - 1; lastKeyframe++) {
        if (curFrame < c.rot_frames[lastKeyframe + 1]) break;
    }
    nextKeyframe = lastKeyframe + 1;

    float keyframeTimeDiff = c.rot_frames[nextKeyframe] - c.rot_frames[lastKeyframe];
    float factor = (curFrame - c.rot_frames[lastKeyframe]) / keyframeTimeDiff;
    factor = glm::clamp(factor, 0.0f, 1.0f);

    glm::quat start = c.rotations[lastKeyframe];
    glm::quat end = c.rotations[nextKeyframe];

    return glm::normalize(glm::slerp(start, end, factor));
}

glm::vec3 AnimationSystem::interpolatePosition(Animation::Channel c, float curFrame)
{
    // Guaranteed that this has keys because it's only called if hasKeys is true
    //Animation::Channel c = anim.getKeys(bone->name);
    if (c.positions.size() == 1) return c.positions[0];
    if (curFrame >= c.pos_frames.back()) return c.positions.back();

    uint lastKeyframe, nextKeyframe;
    for (lastKeyframe = 0; lastKeyframe < c.pos_frames.size() - 1; lastKeyframe++) {
        if (curFrame < c.pos_frames[lastKeyframe + 1]) break;
    }
    nextKeyframe = lastKeyframe + 1;

    float keyframeTimeDiff = c.pos_frames[nextKeyframe] - c.pos_frames[lastKeyframe];
    float factor = (curFrame - c.pos_frames[lastKeyframe]) / keyframeTimeDiff;
    factor = glm::clamp(factor, 0.0f, 1.0f);

    glm::vec3 start = c.positions[lastKeyframe];
    glm::vec3 end = c.positions[nextKeyframe];

    return glm::mix(start, end, factor);
}
