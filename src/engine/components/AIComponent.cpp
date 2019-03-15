#include "AIComponent.h"

#include "engine/ai/BTNode.h"

AIComponent::AIComponent(std::shared_ptr<GameObject> parent, std::shared_ptr<Blackboard> b) :
    Component(parent),
    m_blackboard(b)
{
}

AIComponent::~AIComponent()
{
    delete m_root;
}

std::shared_ptr<Blackboard> AIComponent::getBlackboard()
{
    return m_blackboard;
}

void AIComponent::setRoot(BTNode *n)
{
    m_root = n;
}

void AIComponent::updateAITree(float seconds)
{
    m_root->update(seconds);
}
