#include "RandomSelector.h"

RandomSelector::RandomSelector()
{
}

Status RandomSelector::update(float seconds)
{
    if (m_children.isEmpty()) return FAIL;

    std::uniform_int_distribution<int> selector(0, m_children.length() - 1);
    int to_hit = selector(m_rng);

    return m_children[to_hit]->update(seconds);
}

void RandomSelector::reset()
{
}
