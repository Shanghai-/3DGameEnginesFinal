#ifndef CANIMATEDMESH_H
#define CANIMATEDMESH_H

#include "Component.h"
#include "engine/graphics/RiggedMesh.h"

class CAnimatedMesh : public Component
{
public:
    CAnimatedMesh(std::shared_ptr<GameObject> parent, std::string modelPath);
    CAnimatedMesh(std::shared_ptr<GameObject> parent, std::string modelPath, std::string materialName);
    ~CAnimatedMesh();

    RiggedMesh *getMesh();
    QString getCurrentAnimation();
    void setCurrentAnimation(QString s);

    void resetSeconds();
    float addSeconds(float seconds); // adds seconds to accumulatedSeconds and returns new accumulated value
    float getSeconds();

    bool isFinished();

    bool useModelMaterials();
    std::string getMaterial();

private:
    std::unique_ptr<RiggedMesh> m_mesh;
    std::string m_material;

    QString m_currentAnimation;
    float m_accumulatedSeconds;

    bool m_finished;
};

#endif // CANIMATEDMESH_H
