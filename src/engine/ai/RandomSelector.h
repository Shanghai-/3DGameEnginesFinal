#ifndef RANDOMSELECTOR_H
#define RANDOMSELECTOR_H

#include "CompositeNode.h"

#include <random>

class RandomSelector : public CompositeNode
{
public:
    RandomSelector();

    Status update(float seconds);
    void reset();

private:
    std::mt19937 m_rng;
};

#endif // RANDOMSELECTOR_H
