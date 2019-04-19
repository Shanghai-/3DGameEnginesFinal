#ifndef ARMATURE_H
#define ARMATURE_H

#include <vector>
#include <glm/glm.hpp>
#include <QHash>
#include <QString>

class Armature
{
public:
    struct Bone {
        QString name;
        uint id;

        glm::mat4 offset;
        glm::mat4 transform;
        glm::mat4 finalTransform;

        std::vector<Bone *> children;
    };

    Armature();
    Armature(glm::mat4 globalInverseTransform);
    ~Armature();

    void setRoot(Bone *root);
    std::vector<Bone *> getRoots();

    void goToRestPose();

    void addBone(Bone b);
    bool hasBone(QString name);
    Bone *getBone(QString name);
    Bone *getBone(uint id);
    uint getID(QString name);

    glm::mat4 getGlobalInverse();
    uint getNumBones();

    void print();

private:
    std::vector<Bone *> m_roots;
    QHash<QString, int> m_boneMap;
    std::vector<Bone> m_allBones;

    uint m_num_bones;
    glm::mat4 m_globalInverse;

    void printRecursive(Bone *b);
};

#endif // ARMATURE_H
