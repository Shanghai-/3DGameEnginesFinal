#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "Component.h"
#include "CNavMesh.h"
#include "engine/ai/Blackboard.h"

class AStarNode;
class BTNode;

class AIComponent : public Component
{
public:
    AIComponent(std::shared_ptr<GameObject> parent, std::shared_ptr<Blackboard> b);
    ~AIComponent();

    std::shared_ptr<Blackboard> getBlackboard();

    void setRoot(BTNode *n);
    void updateAITree(float seconds);

private:
    std::shared_ptr<Blackboard> m_blackboard;
    BTNode *m_root;
};

#endif // AICOMPONENT_H
