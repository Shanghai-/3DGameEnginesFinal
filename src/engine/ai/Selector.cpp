#include "Selector.h"

Selector::Selector() :
    m_lastIndex(std::numeric_limits<int>::max())
{
}

Status Selector::update(float seconds)
{
    for (int i = 0; i < m_children.length(); i++) {
        Status result = m_children[i]->update(seconds);

        if (result != FAIL && i < m_lastIndex) {
            m_lastIndex = i;

            for (int j = i + 1; j < m_children.length(); j++) {
                m_children[j]->reset();
            }
            return result;
        }
    }

    m_lastIndex = m_children.length();
    return FAIL;
}

void Selector::reset()
{
    for (int i = 0; i < m_children.length(); i++) {
        m_children[i]->reset();
    }
}
