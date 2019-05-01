#ifndef PLAYERMOVEMENTSYS_H
#define PLAYERMOVEMENTSYS_H

#include "engine/systems/System.h"
#include "engine/input/InputManager.h"
#include "engine/components/CTransform.h"
#include "engine/graphics/Graphics.h"
#include "engine/components/cmeshcol.h"

class PlayerMovementSys : public System
{
public:
    PlayerMovementSys(int priority);
    ~PlayerMovementSys();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);
    void draw();

    void addGlobalMesh(std::shared_ptr<CMeshCol> mesh, glm::ivec2 coord);
    void addMesh(glm::ivec2 coord);


private:
    InputManager *m_input;
    Graphics *m_graphics;

    QSet<std::shared_ptr<CTransform>> m_transforms;

    struct returnType{
        float time;
        glm::vec3 normal;
        glm::vec3 contactPoint;
    };

    returnType checkCollision(glm::vec3 start, glm::vec3 end, glm::vec3 rad);
    glm::vec3 scaleVector(glm::vec3 vector, glm::mat4x4 scale);

    std::shared_ptr<Shape> m_shape;
    void addToVector(std::vector<float> &vector, int index, std::vector<float> values);
    void addToVector(std::vector<int> &vector, int index, std::vector<int> values);

    std::map<std::vector<int>, std::shared_ptr<CMeshCol>> m_globalMeshMap;
    std::map<std::vector<int>, std::shared_ptr<CMeshCol>> m_meshMap;

    std::vector<int> glmToStd(glm::ivec2 vec);
    glm::ivec2 stdToGlm(std::vector<int> vec);

    void removeMesh(glm::ivec2 coord);

    glm::ivec2 m_curPos;
    void updateMeshMap(glm::vec2 pos2D);
    bool m_grounded;
    // System interface
};

#endif // PLAYERMOVEMENTSYS_H
