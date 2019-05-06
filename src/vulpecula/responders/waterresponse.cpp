#include "waterresponse.h"

#include "engine/components/CCollider.h"
#include "engine/components/CRenderable.h"
#include "engine/components/CAudioSource.h"
#include "engine/objectManagement/GameWorld.h"

WaterResponse::WaterResponse()
{
}

WaterResponse::~WaterResponse()
{
}

void WaterResponse::onCollide(std::shared_ptr<GameObject> other)
{
    if(other->getName() == "Player") {
        other->getComponent<CTransform>()->pos = glm::vec3(-15.6f, 3.f, -28.5f);
    }
}
