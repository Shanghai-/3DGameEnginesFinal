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
    void addMesh(std::shared_ptr<CMeshCol> mesh);

private:
    InputManager *m_input;
    Graphics *m_graphics;

    QSet<std::shared_ptr<CTransform>> m_transforms;

    struct returnType{
        float time;
        glm::vec3 normal;
        glm::vec3 contactPoint;
    };

    std::vector<std::shared_ptr<CMeshCol>> m_meshes;
    returnType checkCollision(glm::vec3 start, glm::vec3 end, glm::vec3 rad);
    glm::vec3 scaleVector(glm::vec3 vector, glm::mat4x4 scale);

    std::shared_ptr<Shape> m_shape;
    void addToVector(std::vector<float> &vector, int index, std::vector<float> values);
    void addToVector(std::vector<int> &vector, int index, std::vector<int> values);

    // System interface
};

#endif // PLAYERMOVEMENTSYS_H
