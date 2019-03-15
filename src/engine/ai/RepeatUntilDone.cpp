#include "RepeatUntilDone.h"

RepeatUntilDone::RepeatUntilDone(BTNode *child) :
    m_child(child)
{
}

RepeatUntilDone::~RepeatUntilDone()
{
    delete m_child;
}

Status RepeatUntilDone::update(float seconds)
{
    Status childStatus = m_child->update(seconds);

    while (childStatus == RUNNING) {
        childStatus = m_child->update(seconds);
    }

    return childStatus;
}

void RepeatUntilDone::reset()
{
    m_child->reset();
}
