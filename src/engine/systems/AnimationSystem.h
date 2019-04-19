#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "System.h"

#include "engine/components/CAnimatedMesh.h"

class Graphics;

/**
 * @brief The AnimationSystem class handles updating the armatures of each CAnimatedMesh
 * every frame in tick(), as well as passing the shader uniforms and drawing in draw().
 * It is NOT responsible for changing the current animation of a CAnimatedMesh component.
 * It simply handles the actual matrix math and the drawing.
 */
class AnimationSystem : public System
{
public:
    AnimationSystem(int priority);
    ~AnimationSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);
    void draw();

private:
    QSet<std::shared_ptr<CAnimatedMesh>> m_meshes;
    Graphics *m_graphics;

    void updateBone(Animation anim, Armature::Bone *bone,
                    glm::mat4 parentTransform, glm::mat4 globalInverse,
                    float curFrame);
    glm::vec3 interpolateScale(Animation::Channel c, float curFrame);
    glm::quat interpolateRotation(Animation::Channel c, float curFrame);
    glm::vec3 interpolatePosition(Animation::Channel c, float curFrame);

};

#endif // ANIMATIONSYSTEM_H
