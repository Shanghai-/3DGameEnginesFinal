#ifndef CRenderable_H
#define CRenderable_H

#include "Component.h"
#include "engine/graphics/Graphics.h"

class CRenderable : public Component
{
public:
    CRenderable(std::shared_ptr<GameObject> parent, QString primitiveName);
    CRenderable(std::shared_ptr<GameObject> parent, QString primitiveName, QString materialName);
    ~CRenderable();

    std::shared_ptr<Shape> getShape();
    QString getShapeName();
    QString getMaterialName();

    void setMaterialName(QString n);

private:
    QString m_primitive, m_material;
    Graphics *m_graphics;
};

#endif // CRenderable_H
