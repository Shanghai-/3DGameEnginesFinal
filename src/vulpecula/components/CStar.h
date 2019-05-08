#ifndef CSTAR_H
#define CSTAR_H

#include "engine/components/Component.h"

#include <glm/glm.hpp>

class CStar : public Component
{
public:
    CStar(std::shared_ptr<GameObject> parent, glm::vec3 originalPos);
    ~CStar();

    glm::vec3 getOriginalPosition();

private:
    glm::vec3 m_originalPos;
};

#endif // CSTAR_H
