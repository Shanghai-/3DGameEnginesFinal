#include "Armature.h"

#include <assert.h>
#include <iostream>

Armature::Armature() :
    Armature(glm::mat4())
{
}

Armature::Armature(glm::mat4 globalInverseTransform) :
    m_num_bones(0),
    m_globalInverse(globalInverseTransform)
{
}

Armature::~Armature()
{
}

void Armature::setRoot(Armature::Bone *root)
{
    // The root must already be a bone in the armature;
    // you cannot call setRoot on a new bone
    assert(hasBone(root->name));

    m_roots.push_back(root);
}

std::vector<Armature::Bone *> Armature::getRoots()
{
    return m_roots;
}

void Armature::goToRestPose()
{
    for (uint i = 0; i < m_allBones.size(); i++) {
        Bone *b = &m_allBones[i];
        b->transform = b->offset;
        b->finalTransform = b->offset;
    }
}

void Armature::addBone(Armature::Bone b)
{
    // No duplicates allowed!
    assert(!hasBone(b.name));

    b.id = m_num_bones;
    m_num_bones++;

    m_allBones.push_back(b);
    m_boneMap.insert(b.name, b.id);
}

bool Armature::hasBone(QString name)
{
    return m_boneMap.contains(name);
}

Armature::Bone *Armature::getBone(QString name)
{
    if (hasBone(name)) {
        return &m_allBones[m_boneMap.value(name)];
    }

    return nullptr;
}

Armature::Bone *Armature::getBone(uint id)
{
    if (id < m_num_bones) {
        return &m_allBones[id];
    }

    return nullptr;
}

uint Armature::getID(QString name)
{
    assert(hasBone(name));

    return m_boneMap.value(name);
}

glm::mat4 Armature::getGlobalInverse()
{
    return m_globalInverse;
}

uint Armature::getNumBones()
{
    return m_num_bones;
}

void Armature::print()
{
    for (uint i = 0; i < m_roots.size(); i++) {
        printRecursive(m_roots[i]);
    }
}

void Armature::printRecursive(Bone *b)
{
    std::cout << "Parent node: " << qPrintable(b->name) << std::endl;
    std::cout << qPrintable(b->name) << "'s children: " << std::endl;
    std::cout << "\t";
    for (uint i = 0; i < b->children.size(); i++) {
        std::cout << qPrintable(b->children[i]->name) << ", ";
    }
    std::cout << std::endl;

    for (uint i = 0; i < b->children.size(); i++) {
        printRecursive(b->children[i]);
    }
}
