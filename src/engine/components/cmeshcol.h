#ifndef CMESHCOL_H
#define CMESHCOL_H


#include "Component.h"
#include "engine/graphics/Graphics.h"

class CMeshCol : public Component
{
public:
    CMeshCol(std::shared_ptr<GameObject> parent, QString primitiveName);
    ~CMeshCol();
    std::vector<glm::vec3> getFaces();
    std::vector<glm::vec3> getVertices();
private:
    QString m_primitive;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_faces;

};

#endif // CMESHCOL_H
