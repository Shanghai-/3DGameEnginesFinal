#ifndef RIGGEDMESH_H
#define RIGGEDMESH_H

#include "Texture2D.h"
#include "Animation.h"
#include "Armature.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <vector>
#include <map>
#include <QHash>

class RiggedMesh
{
public:
    RiggedMesh(const std::string &model);
    ~RiggedMesh();

    Animation getAnimation(QString name);
    Armature getArmature();

    void draw();

private:
    #define BONES_PER_VERTEX 4

    struct VertexBoneData
    {
        uint IDs[BONES_PER_VERTEX];
        float weights[BONES_PER_VERTEX];

        void AddBoneData(uint boneID, float weight);
    };

    struct MeshEntry {
        MeshEntry() {
            numIndices = 0;
            baseVertex = 0;
            baseIndex = 0;
            materialIndex = 0;
        }

        unsigned int numIndices;
        unsigned int baseVertex;
        unsigned int baseIndex;
        unsigned int materialIndex;
    };

    enum BUFFER_TYPES {
        INDEX_BUFFER,
        POS_VB,
        NORMAL_VB,
        TEXCOORD_VB,
        BONE_VB,
        NUM_VBs // dummy entry whose numerical value tells us how many buffers we have
        // This is just a bit more maintainable & readable than a magic number
    };

    bool initFromScene(const aiScene *scene);
    void initMesh(uint index, const aiMesh *mesh,
                  std::vector<glm::vec3> &positions,
                  std::vector<glm::vec3> &normals,
                  std::vector<glm::vec2> &texCoords,
                  std::vector<VertexBoneData> &bones,
                  std::vector<uint> &indices);
    void loadBones(uint meshIndex, const aiMesh *mesh, std::vector<VertexBoneData> &bones);
    std::vector<Armature::Bone *> buildArmature(const aiNode *start);
    void loadAnimations(const aiScene *scene);
    //bool initMaterials(const aiScene *scene, const std::string &filename);

    glm::vec3 convertVec3(const aiVector3D &aiVec);
    glm::mat4 convertMatrix(const aiMatrix4x4 &aiMat);
    glm::quat convertQuat(const aiQuaternion &aiQuat);

    GLuint m_VAO;
    GLuint m_buffers[NUM_VBs];

    std::vector<MeshEntry> m_entries;
    std::vector<Texture2D> m_textures;

    // Bone stuff
    //std::map<std::string, uint> m_boneMap; // bone name -> index
    uint m_numBones;
    //std::vector<BoneInfo> m_boneInfo;
    Armature m_armature;
    glm::mat4 m_globalInverseTransform;
    QHash<QString, Animation> m_animations;
};

#endif // RIGGEDMESH_H
