#ifndef NETWORKCOMPONENT_H
#define NETWORKCOMPONENT_H

#include "Component.h"

class NetworkComponent :
        public Component
{
public:
    NetworkComponent(std::shared_ptr<GameObject> parent);
    ~NetworkComponent();
};

#endif // NETWORKCOMPONENT_H
