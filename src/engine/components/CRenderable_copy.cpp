#include "CRenderable.h"

#include <iostream>
#include "engine/graphics/Shape.h"
#include "engine/graphics/ResourceLoader.h"

CRenderable::CRenderable(std::shared_ptr<GameObject> parent, QString primitiveName) :
    CRenderable(parent, primitiveName, "default")
{
}

CRenderable::CRenderable(std::shared_ptr<GameObject> parent, QString primitiveName, QString materialName) :
    Component(parent),
    m_primitive(primitiveName),
    m_material(materialName)
{
    m_graphics = Graphics::getGlobalInstance();

    if (m_graphics->getShape(m_primitive.toStdString()) == nullptr) {
        // The given name isn't an existing shape in our database.
        // Maybe it's a model? We'll try loading it as a path.

        std::shared_ptr<Shape> obj = std::make_shared<Shape>();
        if (ResourceLoader::readObj(m_primitive, obj))
        {
            m_graphics->addShape(m_primitive.toStdString(), obj);
        } else {
            std::cerr << "WARNING: Could not find model or shape '";
            std::cerr << m_primitive.toStdString() << "'." << std::endl;
            std::cerr << "Shape will display as a cube instead." << std::endl;
            m_primitive = "cube";
        }
    }
}

CRenderable::~CRenderable()
{
}

std::shared_ptr<Shape> CRenderable::getShape()
{
    return m_graphics->getShape(m_primitive.toStdString());
}

QString CRenderable::getShapeName()
{
    return m_primitive;
}

QString CRenderable::getMaterialName()
{
    return m_material;
}

void CRenderable::setMaterialName(QString n)
{
    m_material = n;
}
