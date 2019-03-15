#include "Sequence.h"

Sequence::Sequence() :
    m_lastIndex(0)
{
}

Status Sequence::update(float seconds)
{
    for (int i = m_lastIndex; i < m_children.length(); i++) {
        Status result = m_children[i]->update(seconds);

        if (result == FAIL) {
            m_lastIndex = 0;
            return FAIL;

        } else if (result == RUNNING) {
            return RUNNING;
        }

        m_lastIndex = i + 1;
    }
    m_lastIndex = 0;
    return SUCCESS;
}

void Sequence::reset()
{
    for (int i = 0; i < m_children.length(); i++) {
        m_children[i]->reset();
    }
}
