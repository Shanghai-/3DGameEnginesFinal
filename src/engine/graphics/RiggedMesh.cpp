#include "RiggedMesh.h"

#include "engine/graphics/IBO.h"
#include "engine/graphics/VBOAttribMarker.h"
#include "engine/graphics/ShaderAttribLocations.h"
#include "engine/graphics/Graphics.h"

#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <iostream>
#include <assert.h>

// TODO: move these to ShaderAttrib
#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

void RiggedMesh::VertexBoneData::AddBoneData(uint boneID, float weight)
{
    // Loop over our weights list until we find an empty/available slot
    // Store the ID and weight in that slot
    for (uint i = 0; i < BONES_PER_VERTEX; i++) {
        if (weights[i] == 0.0f) {
            IDs[i] = boneID;
            weights[i] = weight;
            return;
        }
    }

    // If we get down here, it means there was no available slot - some vertex
    // has more than 4 bones acting on it.
    //assert(0);
    std::cout << "[WARNING] Bone " << boneID << " has too many weights!" << std::endl;
}

RiggedMesh::RiggedMesh(const std::string &model) :
    m_VAO(0),
    m_numBones(0)
{
    for (unsigned int i = 0; i < NUM_VBs; i++) {
        m_buffers[i] = 0;
    }

    // get aiScene
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(model, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        std::cerr << "ERROR: Asset import failed with error: " << importer.GetErrorString() << std::endl;
        std::cerr << "The offending file path was '" << model << "'" << std::endl;
    } else {
        m_armature = Armature(glm::inverse(convertMatrix(scene->mRootNode->mTransformation)));

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        glGenBuffers(NUM_VBs, m_buffers);

        initFromScene(scene);

        glBindVertexArray(0);
    }


}

RiggedMesh::~RiggedMesh()
{
    if (m_buffers[0] != 0) {
        glDeleteBuffers(NUM_VBs, m_buffers);
    }

    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}

Animation RiggedMesh::getAnimation(QString name)
{
    return m_animations.value(name);
}

Armature RiggedMesh::getArmature()
{
    return m_armature;
}

void RiggedMesh::draw()
{
    glBindVertexArray(m_VAO);
    /*
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER]);

    // Bind and enable POSITION
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POS_VB]);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind and enable UVs
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEXCOORD_VB]);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind and enable NORMALS
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_VB]);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind and enable BONES
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BONE_VB]);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, BONES_PER_VERTEX, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
    */

    for (uint i = 0; i < m_entries.size(); i++) {
        // TODO: Material setting
        const uint materialIndex = m_entries[i].materialIndex;

        /* std::cout << m_entries[i].numIndices << std::endl;
        std::cout << m_entries[i].baseVertex << std::endl;
        std::cout << m_entries[i].baseIndex << std::endl;
        std::cout << m_entries[i].materialIndex << std::endl; */
//        glDrawArrays(GL_TRIANGLES, 0, m_entries[i].numIndices);
//        std::vector<GLuint> datums;
//        datums.resize(m_entries[i].numIndices);
//        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_entries[i].numIndices * sizeof(GLuint), datums.data());
//        bool nonzeroFound = false;
//        for (int i = 0; i < datums.size(); i++) {
//            if (datums[i] != 0) {
//                nonzeroFound = true;
//                break;
//            }
//        }
//        if (!nonzeroFound) {
//            glBindVertexArray(0);
//            return;
//        }

        glDrawElementsBaseVertex(GL_TRIANGLES,              // Mode
                                 m_entries[i].numIndices,   // Count
                                 GL_UNSIGNED_INT,           // Type
                                 (void *)(sizeof(uint) * m_entries[i].baseIndex), // Indices pointer
                                 m_entries[i].baseVertex);  // Base vertex
    }

    glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool RiggedMesh::initFromScene(const aiScene *scene)
{
    m_entries.resize(scene->mNumMeshes);
    //m_textures.resize(scene->mNumMaterials);

//    std::cout << "Num meshes: " << scene->mNumMeshes << std::endl;
//    std::cout << "Num animations: " << scene->mNumAnimations << std::endl;
//    std::cout << "Num textures: " << scene->mNumTextures << std::endl;

    // Initialize all the data vectors for our vertices
    // These will turn into GL buffers later
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<VertexBoneData> bones;
    std::vector<uint> indices;

    uint numVertices = 0;
    uint numIndices = 0;

    // Count the number of vertices and indices in the whole model
    for (uint i = 0; i < scene->mNumMeshes; i++) {
        m_entries[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
        m_entries[i].numIndices = scene->mMeshes[i]->mNumFaces * 3;
        m_entries[i].baseVertex = numVertices;
        m_entries[i].baseIndex = numIndices;

        numVertices += scene->mMeshes[i]->mNumVertices;
        numIndices  += m_entries[i].numIndices;
    }

    // Reserve space in the vectors for the vertex attributes and indices
    positions.reserve(numVertices);
    normals.reserve(numVertices);
    texCoords.reserve(numVertices);
    bones.resize(numVertices);
    indices.reserve(numIndices);

    for (uint i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh *mesh = scene->mMeshes[i];
        initMesh(i, mesh, positions, normals, texCoords, bones, indices);
    }

    buildArmature(scene->mRootNode);

    // If material loading fails, import failed
    // TODO: Material loading
    //if (!initMaterials(scene, filename)) return false;

    // Animation loading
    if (scene->HasAnimations()) loadAnimations(scene);

    // We don't use the VBO object here, but we probably could
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, BONES_PER_VERTEX, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    /*
    // Create the buffers for the vertices attributes
    glGenBuffers(NUM_VBs, m_buffers);

    // Position buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);

    // Texture/UVs buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);

    // Normals buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);

    // Bones buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BONE_VB]);
    std::cout << "bones array size: " << bones.size() << std::endl;
    std::cout << "bone size: " << sizeof(bones[0]) << std::endl;
    std::cout << "boneData size: " << sizeof(VertexBoneData) << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
    // TODO: make 16 in the call above into a variable dependent on BONES_PER_VERTEX and sizeof(VertexBoneData)

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Bind and enable POSITION
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POS_VB]);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind and enable UVs
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEXCOORD_VB]);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind and enable NORMALS
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_VB]);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind and enable BONES
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BONE_VB]);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, BONES_PER_VERTEX, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    // Bind index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER]);

    // Clean up
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    */

    return true; // TODO: error check
}

void RiggedMesh::initMesh(uint index, const aiMesh *mesh,
                          std::vector<glm::vec3> &positions,
                          std::vector<glm::vec3> &normals,
                          std::vector<glm::vec2> &texCoords,
                          std::vector<RiggedMesh::VertexBoneData> &bones,
                          std::vector<uint> &indices)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        const aiVector3D pos = mesh->mVertices[i];
        const aiVector3D norm = mesh->mNormals[i];
        const aiVector3D uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : Zero3D;

        positions.push_back(glm::vec3(pos.x, pos.y, pos.z));
        normals.push_back(glm::vec3(norm.x, norm.y, norm.z));
        texCoords.push_back(glm::vec2(uv.x, uv.y));
    }

    loadBones(index, mesh, bones);

    // Populate the index buffer
    for (uint i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
}

void RiggedMesh::loadBones(uint meshIndex, const aiMesh *mesh, std::vector<RiggedMesh::VertexBoneData> &bones)
{
    std::cout << "LOADING BONES" << std::endl;
    for (uint i = 0; i < mesh->mNumBones; i++) {
        QString boneName(mesh->mBones[i]->mName.data);

        // We create the bone objects here, but we don't set up the parent/child relations until later
        if (!m_armature.hasBone(boneName)) {
            Armature::Bone b;
            b.offset = convertMatrix(mesh->mBones[i]->mOffsetMatrix);
            b.transform = glm::mat4(1.0f); // Default value
            b.finalTransform = glm::mat4(1.0f); // Also default
            b.name = boneName;
            m_armature.addBone(b);

            std::cout << "adding " << qPrintable(b.name) << " to armature" << std::endl;
        }

        int boneIndex = m_armature.getID(boneName);

        for (uint j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
            uint vertexID = m_entries[meshIndex].baseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
            float weight = mesh->mBones[i]->mWeights[j].mWeight;
            bones[vertexID].AddBoneData(boneIndex, weight);
        }
    }
}

std::vector<Armature::Bone *> RiggedMesh::buildArmature(const aiNode *start)
{
    QString name(start->mName.data);

    std::vector<Armature::Bone *> ret;

    // LEAF CASE: We're at the end of the tree
    if (start->mNumChildren == 0) {

        // If the tree ends in a bone, we need to save it on our unhandled list
        if (m_armature.hasBone(name)) {
            ret.push_back(m_armature.getBone(name));
        }

        return ret;
    }

    // RECURSIVE CASE: Run on all children first
    for (uint i = 0; i < start->mNumChildren; i++) {
        std::vector<Armature::Bone *> res = buildArmature(start->mChildren[i]);
        for (uint j = 0; j < res.size(); j++) {
            ret.push_back(res[j]);
        }
    }

    // If the current node is a bone, all unhandled child bones belong to it
    if (m_armature.hasBone(name)) {
        Armature::Bone *b = m_armature.getBone(name);
        for (uint i = 0; i < ret.size(); i++) {
            b->children.push_back(ret[i]);
        }
        // Those bones are now handled, so we don't need to propagate them
        ret.clear();

        // But we need someone to handle this bone instead
        ret.push_back(b);
    }

    // If we've reached the top of the tree, assume any remaining unhandled
    // bones are roots
    if (start->mParent == nullptr) {
        if (!ret.empty()) {
            for (uint i = 0; i < ret.size(); i++) {
                m_armature.setRoot(ret[i]);
            }
        }
    }

    return ret;
}

void RiggedMesh::loadAnimations(const aiScene *scene)
{
    m_animations.reserve(scene->mNumAnimations);

    for (uint i = 0; i < scene->mNumAnimations; i++) {
        aiAnimation *anim = scene->mAnimations[i];

        Animation a;
        if (strlen(anim->mName.data) == 0) {
            a = Animation("Default");
        } else {
            a = Animation(anim->mName.data);
        }

        std::cout << "ANIMATION LOADING: " << qPrintable(a.getName()) << std::endl;

        a.setDuration(anim->mDuration);
        a.setTicksPerSec(anim->mTicksPerSecond);

        for (uint j = 0; j < anim->mNumChannels; j++) {
            aiNodeAnim *bone = anim->mChannels[j];

            Animation::Channel c;
            QString boneName = bone->mNodeName.data;

            // Position change frames
            c.positions.reserve(bone->mNumPositionKeys);
            c.pos_frames.reserve(bone->mNumPositionKeys);
            for (uint k = 0; k < bone->mNumPositionKeys; k++) {
                c.positions.push_back(convertVec3(bone->mPositionKeys[k].mValue));
                c.pos_frames.push_back(bone->mPositionKeys[k].mTime);
            }

            // Rotation change frames
            c.rotations.reserve(bone->mNumRotationKeys);
            c.rot_frames.reserve(bone->mNumRotationKeys);
            for (uint k = 0; k < bone->mNumRotationKeys; k++) {
                c.rotations.push_back(convertQuat(bone->mRotationKeys[k].mValue));
                c.rot_frames.push_back(bone->mRotationKeys[k].mTime);
            }

            // Scale change frames
            c.scales.reserve(bone->mNumScalingKeys);
            c.scale_frames.reserve(bone->mNumScalingKeys);
            for (uint k = 0; k < bone->mNumScalingKeys; k++) {
                c.scales.push_back(convertVec3(bone->mScalingKeys[k].mValue));
                c.scale_frames.push_back(bone->mScalingKeys[k].mTime);
            }

            a.addChannel(boneName, c);
        }

        m_animations.insert(a.getName(), a);
    }
}

glm::vec3 RiggedMesh::convertVec3(const aiVector3D &aiVec)
{
    return glm::vec3(aiVec.x, aiVec.y, aiVec.z);
}

inline glm::mat4 RiggedMesh::convertMatrix(const aiMatrix4x4 &aiMat)
{
    return glm::transpose(glm::make_mat4(&aiMat.a1));
}

inline glm::quat RiggedMesh::convertQuat(const aiQuaternion &aiQuat)
{
    return glm::quat(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
}
