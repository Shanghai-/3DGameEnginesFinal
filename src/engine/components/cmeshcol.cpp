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

    if (ResourceLoader::readObj(m_primitive, m_vertices, m_faces))
    {
        std::cout << m_faces.size() << std::endl;
        //m_graphics->addShape(m_primitive.toStdString(), obj);
    } else {
        std::cerr << "WARNING: Could not find model or shape '";
        std::cerr << m_primitive.toStdString() << "'." << std::endl;
        std::cerr << "Shape will display as a cube instead." << std::endl;
        m_primitive = "cube";
    }
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
