#ifndef PLAYERMOVEMENTSYS_H
#define PLAYERMOVEMENTSYS_H

#include "engine/systems/System.h"
#include "engine/input/InputManager.h"
#include "engine/components/CTransform.h"
#include "engine/graphics/Graphics.h"

class PlayerMovementSys : public System
{
public:
    PlayerMovementSys(int priority);
    ~PlayerMovementSys();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    InputManager *m_input;
    Graphics *m_graphics;

    QSet<std::shared_ptr<CTransform>> m_transforms;
};

#endif // PLAYERMOVEMENTSYS_H
