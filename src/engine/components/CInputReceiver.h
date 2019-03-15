#ifndef CINPUTRECEIVER_H
#define CINPUTRECEIVER_H

#include "Component.h"

/**
 * @brief The CInputReceiver class is a general-purpose component
 * which indicates to the System allocator that the owner GameObject
 * should be passed to any input-handling Systems present in the
 * GameWorld.
 */
class CInputReceiver : public Component
{
public:
    CInputReceiver(std::shared_ptr<GameObject> parent);
    ~CInputReceiver();
};

#endif // CINPUTRECEIVER_H
