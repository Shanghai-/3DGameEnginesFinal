#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "engine/graphics/Graphics.h"
#include "engine/graphics/TextureCube.h"
#include "engine/components/CRenderable.h"
#include "System.h"

class RenderSystem : public System
{
public:
    RenderSystem(int priority);
    ~RenderSystem();

    void draw();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);

    void setSkybox(std::shared_ptr<TextureCube> c);

private:
    bool behindPlane(glm::vec4 planeConstants, QVector<glm::vec4> *points);

    Graphics *m_graphics;
    QSet<std::shared_ptr<CRenderable>> m_renderComponents;

    std::shared_ptr<TextureCube> m_skyboxTex;
};

#endif // RENDERSYSTEM_H
