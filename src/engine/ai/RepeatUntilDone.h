#ifndef REPEATUNTILDONE_H
#define REPEATUNTILDONE_H

#include "BTNode.h"

class RepeatUntilDone : public BTNode
{
public:
    RepeatUntilDone(BTNode *child);
    ~RepeatUntilDone();

    Status update(float seconds);
    void reset();

private:
    BTNode *m_child;
};

#endif // REPEATUNTILDONE_H
