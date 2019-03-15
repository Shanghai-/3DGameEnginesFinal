#ifndef BACKAWAY_H
#define BACKAWAY_H

#include "engine/ai/Action.h"

class AIComponent;

class BackAway : public Action
{
public:
    BackAway(AIComponent *owner);

    Status act(float seconds);
    void reset();

private:
    AIComponent *m_owner;
};

#endif // BACKAWAY_H
