#ifndef CNAVMESH_H
#define CNAVMESH_H

#include "Component.h"

#include <QSet>

class AStarNode;

class CNavMesh : public Component
{
public:
    CNavMesh(std::shared_ptr<GameObject> parent, QSet<AStarNode *> nodes);
    CNavMesh(std::shared_ptr<GameObject> parent, AStarNode *root);
    ~CNavMesh();

    QSet<AStarNode *> getNodes();

protected:
    QSet<AStarNode *> m_nodes;
};

#endif // CNAVMESH_H
