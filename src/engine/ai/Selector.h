#ifndef SELECTOR_H
#define SELECTOR_H

#include "CompositeNode.h"

class Selector : public CompositeNode
{
public:
    Selector();    

    Status update(float seconds);
    void reset();

private:
    int m_lastIndex;
};

#endif // SELECTOR_H
