#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "engine/systems/System.h"
#include "engine/components/CInputReceiver.h"

class InputManager;
class Graphics;

class InputSystem : public System
{
public:
    InputSystem(int priority);
    ~InputSystem();

    QString getComponentType() const;
    void addComponent(const std::shared_ptr<Component> &c);
    void removeComponent(const std::shared_ptr<Component> &c);
    void tick(float seconds);

private:
    InputManager *m_input;
    Graphics *m_graphics;

    QSet<std::shared_ptr<CInputReceiver>> m_components;
};

#endif // INPUTSYSTEM_H
