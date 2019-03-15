#ifndef ASTARSEARCH_H
#define ASTARSEARCH_H

#include "AStarNode.h"
#include "glm/gtx/norm.hpp"

#include <QSet>
#include <queue>
#include <vector>

class AStarSearch
{
public:
    static const QVector<AStarNode *> getPath(AStarNode *start, AStarNode *end)
    {
        QVector<AStarNode *> path = QVector<AStarNode *>();

        if (!isConnected(start, end)) return path;

        AStarNode *curr = end;
        while (curr->prev != nullptr) {
            path.prepend(curr);
            curr = curr->prev;
        }
        path.prepend(curr);

        return path;
    }

private:
    struct NodePtrCompare
    {
        bool operator()(const AStarNode *lhs, const AStarNode *rhs) const
        {
            return lhs->cost > rhs->cost;
        }
    };

    static bool isConnected(AStarNode *start, AStarNode *end)
    {
        if (start == end || start->getPosition() == end->getPosition()) {
            return false;
        }

        std::priority_queue<AStarNode *, std::vector<AStarNode *>, NodePtrCompare> toExplore;
        // c++ won't let us check if a value is inside the pq, so we have to hack it with a second extra set
        QSet<AStarNode *> explored, toExploreContents;

        start->cost = 0.0f;
        start->prev = nullptr;
        toExplore.push(start);
        toExploreContents.insert(start);

        while (!toExplore.empty()) {
            AStarNode *curr = toExplore.top();
            toExplore.pop();
            if (curr == nullptr) return false;

            toExploreContents.remove(curr);

            if (explored.contains(curr)) continue;

            if (curr == end) {
                return true;
            }

            explored.insert(curr);

            QVectorIterator<AStarNode *> it(curr->getEdges());
            while (it.hasNext()) {
                AStarNode *other = it.next();

                float cost = curr->cost + glm::distance2(curr->getPosition(), other->getPosition())
                        + glm::distance2(other->getPosition(), end->getPosition());

                if (!explored.contains(other)) {
                    other->cost = cost;
                    other->prev = curr;
                    toExplore.push(other);
                    toExploreContents.insert(other);
                }
            }
        }
        return false;
    }
};

#endif // ASTARSEARCH_H
