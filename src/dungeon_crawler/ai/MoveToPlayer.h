#ifndef MOVETOPLAYER_H
#define MOVETOPLAYER_H

#include "engine/ai/Action.h"
#include "engine/ai/AStarNode.h"

class AIComponent;

class MoveToPlayer : public Action
{
public:
    MoveToPlayer(AIComponent *owner);

    Status act(float seconds);
    void reset();

private:
    AIComponent *m_owner;
    QVector<AStarNode *> m_path;
};

#endif // MOVETOPLAYER_H
