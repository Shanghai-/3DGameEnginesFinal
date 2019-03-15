#include "Inverter.h"

Inverter::Inverter(BTNode *child) :
    m_child(child)
{
}

Inverter::~Inverter()
{
    delete m_child;
}

Status Inverter::update(float seconds)
{
    Status childStatus = m_child->update(seconds);

    if (childStatus == FAIL) return SUCCESS;
    if (childStatus == SUCCESS) return FAIL;

    return childStatus;
}

void Inverter::reset()
{
    m_child->reset();
}
