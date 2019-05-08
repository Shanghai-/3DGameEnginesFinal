#include "waterresponse.h"

#include "engine/components/CCollider.h"
#include "engine/components/CRenderable.h"
#include "engine/components/CAudioSource.h"
#include "engine/objectManagement/GameWorld.h"


WaterResponse::WaterResponse(std::shared_ptr<PlayerMovementSys> mps)
{
    m_mps = mps;
}

WaterResponse::~WaterResponse()
{
}

void WaterResponse::onCollide(std::shared_ptr<GameObject> other)
{
    std::cout << "collision" << std::endl;
    if(other->getName() == "Player") {
        std::cout << "player collision" << std::endl;
        other->getComponent<CTransform>()->pos = glm::vec3(-15.6f, 3.f, -28.5f);
    }

    m_mps->updateWater();


}
