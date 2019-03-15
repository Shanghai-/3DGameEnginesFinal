#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "engine/components/Component.h"

#include <QHash>
#include <QString>

class Blackboard
{
public:
    Blackboard();
    ~Blackboard();

    void addComponent(QString name, std::shared_ptr<Component> comp);
    void removeComponent(QString name);

    template <class T>
    std::shared_ptr<T> getComponent(QString name) const
    {
        return std::dynamic_pointer_cast<T>(m_store.value(name));
    }

private:
    QHash<QString, std::shared_ptr<Component>> m_store;
};

#endif // BLACKBOARD_H
