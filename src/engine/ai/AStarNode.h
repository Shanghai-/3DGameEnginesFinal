#ifndef ASTARNODE_H
#define ASTARNODE_H

#include "glm/glm.hpp"

#include <QVector>

class AStarNode
{
public:
    AStarNode(glm::vec3 position);

    void addEdge(AStarNode *n, bool symmetrical);
    void removeEdge(AStarNode *n, bool symmetrical);

    const QVector<AStarNode *> getEdges();

    const glm::vec3 getPosition();

    // For searching
    float cost;
    AStarNode *prev;
    bool onPath;

private:
    glm::vec3 m_pos;
    QVector<AStarNode *> m_edges;
};

#endif // ASTARNODE_H
