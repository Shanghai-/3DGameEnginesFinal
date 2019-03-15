#ifndef ACTION_H
#define ACTION_H

#include "BTNode.h"

class Action : public BTNode
{
public:
    Status update(float seconds)
    {
        return act(seconds);
    }

    virtual Status act(float seconds) = 0;
};

#endif // ACTION_H
