#ifndef RADIUSCONDITION_H
#define RADIUSCONDITION_H

#include "engine/ai/Condition.h"
#include "engine/ai/Blackboard.h"

class AIComponent;

class RadiusCondition : public Condition
{
public:
    RadiusCondition(AIComponent *owner, float r);

    Status query();
    void reset();

private:
    AIComponent *m_owner;
    float m_radius;
};

#endif // RADIUSCONDITION_H
