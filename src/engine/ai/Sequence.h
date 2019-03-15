#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "CompositeNode.h"

class Sequence : public CompositeNode
{
public:
    Sequence();

    Status update(float seconds);
    void reset();

private:
    int m_lastIndex;
};

#endif // SEQUENCE_H
