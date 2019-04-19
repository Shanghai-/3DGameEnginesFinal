#include "CAnimatedMesh.h"

#include "engine/graphics/Animation.h"

CAnimatedMesh::CAnimatedMesh(std::shared_ptr<GameObject> parent, std::__cxx11::string modelPath) :
    CAnimatedMesh(parent, modelPath, "")
{
}

CAnimatedMesh::CAnimatedMesh(std::shared_ptr<GameObject> parent, std::__cxx11::string modelPath, std::__cxx11::string materialName) :
    Component(parent),
    m_material(materialName),
    m_currentAnimation("Armature|Walk"), // TODO: this is just default
    //m_currentAnimation("Default"), // TODO: this is just default
    //m_currentAnimation("None"),
    m_accumulatedSeconds(0.0f)
{
    m_mesh = std::make_unique<RiggedMesh>(modelPath);
}

CAnimatedMesh::~CAnimatedMesh()
{
}

RiggedMesh *CAnimatedMesh::getMesh()
{
    return m_mesh.get();
}

QString CAnimatedMesh::getCurrentAnimation()
{
    return m_currentAnimation;
}

void CAnimatedMesh::setCurrentAnimation(QString s)
{
    m_currentAnimation = s;
}

void CAnimatedMesh::resetSeconds()
{
    m_accumulatedSeconds = 0.0f;
}

float CAnimatedMesh::addSeconds(float seconds)
{
    m_accumulatedSeconds += seconds;
    return m_accumulatedSeconds;
}

float CAnimatedMesh::getSeconds()
{
    return m_accumulatedSeconds;
}

bool CAnimatedMesh::isFinished()
{
    if (m_currentAnimation == "None") return true;

    Animation a = m_mesh->getAnimation(m_currentAnimation);
    return (!a.shouldLoop() && m_accumulatedSeconds * a.getTicksPerSec() > a.getDuration());
}

bool CAnimatedMesh::useModelMaterials()
{
    return m_material.empty();
}

std::__cxx11::string CAnimatedMesh::getMaterial()
{
    return m_material;
}
