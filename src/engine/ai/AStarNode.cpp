#include "AStarNode.h"

AStarNode::AStarNode(glm::vec3 position) :
    m_pos(position),
    cost(0.0f),
    prev(nullptr)
{
}

void AStarNode::addEdge(AStarNode *n, bool symmetrical)
{
    m_edges.append(n);
    if (symmetrical) n->addEdge(this, false);
}

void AStarNode::removeEdge(AStarNode *n, bool symmetrical)
{
    m_edges.removeAt(m_edges.indexOf(n));

    if (symmetrical) n->removeEdge(this, false);
}

const QVector<AStarNode *> AStarNode::getEdges()
{
    return m_edges;
}

const glm::vec3 AStarNode::getPosition()
{
    return m_pos;
}
