#ifndef CCOOLDOWN_H
#define CCOOLDOWN_H

#include "engine/components/Component.h"

class CCooldown : public Component
{
public:
    CCooldown(std::shared_ptr<GameObject> parent, float seconds);
    ~CCooldown();

    void decrease(float seconds);
    void begin();
    bool isReady();

private:
    float m_cooldownLength;
    float m_currSeconds;
};

#endif // CCOOLDOWN_H
