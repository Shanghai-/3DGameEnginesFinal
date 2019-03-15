#ifndef CONDITION_H
#define CONDITION_H

#include "BTNode.h"

class Condition : public BTNode
{
public:
    Status update(float seconds)
    {
        return query();
    }

    virtual Status query() = 0;
};

#endif // CONDITION_H
