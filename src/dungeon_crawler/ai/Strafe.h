#ifndef STRAFE_H
#define STRAFE_H

#include "engine/ai/Action.h"

class AIComponent;

class Strafe : public Action
{
public:
    Strafe(AIComponent *owner);

    Status act(float seconds);
    void reset();

private:
    AIComponent *m_owner;
};

#endif // STRAFE_H
