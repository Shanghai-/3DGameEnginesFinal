#ifndef STARSPINNER_H
#define STARSPINNER_H

#include "engine/systems/System.h"
#include "vulpecula/components/CStar.h"

class StarSpinner : public System
{
public:
    StarSpinner(int priority);
    ~StarSpinner();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    QSet<std::shared_ptr<CStar>> m_stars;
};

#endif // STARSPINNER_H
