#ifndef CAUDIOSOURCE_H
#define CAUDIOSOURCE_H

#include "Component.h"

class CAudioSource : public Component
{
public:
    CAudioSource(std::shared_ptr<GameObject> parent);
    ~CAudioSource();

private:

};

#endif // CAUDIOSOURCE_H
