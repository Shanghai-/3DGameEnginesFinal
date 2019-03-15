#ifndef NAVMESHDRAWER_H
#define NAVMESHDRAWER_H

#include "engine/systems/System.h"

#include "engine/components/CNavMesh.h"

class Graphics;

class NavmeshDrawer : public System
{
public:
    NavmeshDrawer(int priority);
    ~NavmeshDrawer();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void draw();

private:
    QSet<std::shared_ptr<CNavMesh>> m_meshes;
    Graphics *m_graphics;
};

#endif // NAVMESHDRAWER_H
