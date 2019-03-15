#ifndef INVERTER_H
#define INVERTER_H

#include "BTNode.h"

class Inverter : public BTNode
{
public:
    Inverter(BTNode *child);
    ~Inverter();

    Status update(float seconds);
    void reset();

private:
    BTNode *m_child;
};

#endif // INVERTER_H
