#include "CNavMesh.h"

#include "engine/ai/AStarNode.h"

#include <iostream>
#include "glm/gtx/string_cast.hpp"

#include <QQueue>

CNavMesh::CNavMesh(std::shared_ptr<GameObject> parent, QSet<AStarNode *> nodes) :
    Component(parent),
    m_nodes(nodes)
{
}

CNavMesh::CNavMesh(std::shared_ptr<GameObject> parent, AStarNode *root) :
    Component(parent)
{
    std::cout << glm::to_string(root->getPosition()) << std::endl;

    QQueue<AStarNode *> to_explore;
    to_explore.enqueue(root);

    while (!to_explore.isEmpty())
    {
        AStarNode *curr = to_explore.dequeue();

        if (!m_nodes.contains(curr)) {
            m_nodes.insert(curr);
            QVectorIterator<AStarNode *> edges(curr->getEdges());
            while (edges.hasNext()) {
                to_explore.append(edges.next());
            }
        }
    }
}

CNavMesh::~CNavMesh()
{
    QSetIterator<AStarNode *> it(m_nodes);
    while (it.hasNext()) {
        AStarNode *curr = it.next();

        QVectorIterator<AStarNode *> edges(curr->getEdges());
        while (edges.hasNext()) {
            curr->removeEdge(edges.next(), true);
        }

        delete curr;
    }
}

QSet<AStarNode *> CNavMesh::getNodes()
{
    return m_nodes;
}
