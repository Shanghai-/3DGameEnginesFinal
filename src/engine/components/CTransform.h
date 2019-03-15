#ifndef CTRANSFORM_H
#define CTRANSFORM_H

#include "engine/components/Component.h"
#include "glm/glm.hpp"

class CTransform : public Component
{
public:
    CTransform(std::shared_ptr<GameObject> parent, bool unmoving);
    CTransform(std::shared_ptr<GameObject> parent, bool unmoving, glm::vec3 pos);
    CTransform(std::shared_ptr<GameObject> parent, bool unmoving,
               glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
    ~CTransform();

    bool is_static() const;

    QString getRequiredSystem();

    void translate(glm::vec3 translation);
    void rotate(glm::vec3 rotation);
    void scaleBy(glm::vec3 s);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 s);

    glm::vec3 toWorld(glm::vec3 localPos);

    glm::vec3 pos, rot, scale;

private:
    const bool m_static;
};

#endif // CTRANSFORM_H
