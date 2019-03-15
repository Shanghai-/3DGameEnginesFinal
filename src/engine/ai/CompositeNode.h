#ifndef COMPOSITENODE_H
#define COMPOSITENODE_H

#include "BTNode.h"

#include <QVector>
#include <memory>

class CompositeNode : public BTNode
{
public:
    ~CompositeNode() {
        int sz = m_children.size();
        for (int i = 0; i < sz; i++) {
            delete m_children[i];
        }
        m_children.clear();
    }

    void addChild(BTNode *n) { m_children.append(n); }

protected:
    QVector<BTNode *> m_children;
};

#endif // COMPOSITENODE_H
