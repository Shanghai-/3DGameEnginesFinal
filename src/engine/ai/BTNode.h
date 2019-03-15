#ifndef BTNODE_H
#define BTNODE_H

#include "engine/components/AIComponent.h"

#include <QHash>

enum Status { SUCCESS, FAIL, RUNNING };

class BTNode
{
public:
    BTNode() {}
    ~BTNode() {}

    virtual Status update(float seconds) = 0;
    virtual void reset() = 0;
};

#endif // BTNODE_H
