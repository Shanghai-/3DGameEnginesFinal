#ifndef CHEALTH_H
#define CHEALTH_H

#include "engine/components/Component.h"

class CHealth : public Component
{
public:
    CHealth(std::shared_ptr<GameObject> parent, int max);
    ~CHealth();

    void addHealth(int to_add, bool overheal);
    void removeHealth(int to_remove);

    int getMaxHealth();
    int getCurrHealth();

private:
    int m_maxHealth;
    int m_currHealth;
};

#endif // CHEALTH_H
