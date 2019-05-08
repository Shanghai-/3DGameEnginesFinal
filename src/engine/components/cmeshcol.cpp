#include "cmeshcol.h"

#include <iostream>
#include "engine/graphics/Shape.h"
#include "engine/graphics/ResourceLoader.h"
#include "engine/graphics/Shape.h"
#include "engine/components/CTransform.h"

CMeshCol::CMeshCol(std::shared_ptr<GameObject> parent, QString primitiveName) :
    Component(parent),
    m_primitive(primitiveName)
{

    if (ResourceLoader::readObj(m_primitive, m_vertices, m_faces, m_normals))
    {
        std::vector<float> shapeVertices(8 * m_vertices.size());
        std::vector<int> shapeFaces(3 * m_faces.size());
        for(int i = 0; i < m_faces.size(); i++) {
            glm::vec3 verts = m_faces[i];
            int ind = 3 * i;
            shapeFaces[ind] = verts[0];
            shapeFaces[ind + 1] = verts[1];
            shapeFaces[ind + 2] = verts[2];

            glm::vec3 norm = m_normals[i];

            ind = 8 * verts[0];
            glm::vec3 pos = m_vertices[verts[0]];
            shapeVertices[ind] = pos[0];
            shapeVertices[ind + 1] = pos[1];
            shapeVertices[ind + 2] = pos[2];
            shapeVertices[ind + 3] = norm[0];
            shapeVertices[ind + 4] = norm[1];
            shapeVertices[ind + 5] = norm[2];
            shapeVertices[ind + 6] = 0.f;
            shapeVertices[ind + 7] = 1.f;

            ind = 8 * verts[1];
            pos = m_vertices[verts[1]];
            shapeVertices[ind] = pos[0];
            shapeVertices[ind + 1] = pos[1];
            shapeVertices[ind + 2] = pos[2];
            shapeVertices[ind + 3] = norm[0];
            shapeVertices[ind + 4] = norm[1];
            shapeVertices[ind + 5] = norm[2];
            shapeVertices[ind + 6] = 0.f;
            shapeVertices[ind + 7] = 1.f;

            ind = 8 * verts[2];
            pos = m_vertices[verts[2]];
            shapeVertices[ind] = pos[0];
            shapeVertices[ind + 1] = pos[1];
            shapeVertices[ind + 2] = pos[2];
            shapeVertices[ind + 3] = norm[0];
            shapeVertices[ind + 4] = norm[1];
            shapeVertices[ind + 5] = norm[2];
            shapeVertices[ind + 6] = 0.f;
            shapeVertices[ind + 7] = 1.f;
        }
        m_shape = std::make_shared<Shape>(shapeVertices, shapeFaces);

    } else {
        std::cerr << "WARNING: Could not find model or shape '";
        std::cerr << m_primitive.toStdString() << "'." << std::endl;
        std::cerr << "Shape will display as a cube instead." << std::endl;
        m_primitive = "cube";
    }
}

CMeshCol::CMeshCol(std::shared_ptr<GameObject> parent) : Component(parent)
{
    m_vertices.push_back(glm::vec3(-10.f, 10.f, -10.f));
    m_vertices.push_back(glm::vec3(-10.f, 10.f, 10.f));
    m_vertices.push_back(glm::vec3(10.f, 15.f, -10.f));
    m_vertices.push_back(glm::vec3(10.f, 15.f, 10.f));

    m_faces.push_back(glm::vec3(0, 1, 2));
    m_faces.push_back(glm::vec3(1, 3, 2));

    glm::vec3 v0 = m_vertices[0];
    glm::vec3 v1 = m_vertices[1];
    glm::vec3 v2 = m_vertices[2];
    glm::vec3 v3 = m_vertices[3];

    m_normals.push_back(glm::normalize(glm::cross(v1 - v0, v2 - v0)));
    m_normals.push_back(glm::normalize(glm::cross(v3 - v1, v2 - v1)));
}

CMeshCol::~CMeshCol()
{
}

std::vector<glm::vec3> CMeshCol::getFaces() {
    return m_faces;
}

std::vector<glm::vec3> CMeshCol::getVertices() {
    return m_vertices;
}

std::vector<glm::vec3> CMeshCol::getNormals()
{
    return m_normals;
}

void CMeshCol::drawWireframe()
{
    Graphics *g = Graphics::getGlobalInstance();

    g->clearTransform();
    g->drawShape(m_shape);
}
