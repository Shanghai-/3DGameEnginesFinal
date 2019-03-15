#ifndef CCOLLIDER_H
#define CCOLLIDER_H

#include "Component.h"
#include "CTransform.h"
#include "volumes/CollisionVolume.h"
#include "responders/CollisionResponse.h"

#include <glm/glm.hpp>

/**
 * @brief The CCollider class
 */
class CCollider : public Component
{
public:
    /**
     * @brief The COLLISION_LAYERS enum provides some basic layers
     * for collision management. Additional layers can be added
     * by simply using numbers instead of these enum values.
     */
    enum COLLISION_LAYERS {
        DEFAULT, WORLD, INTERACTIVE
    };

    /**
     * @brief CCollider constructs a collider component
     * @param parent - the GameObject this collider belongs to
     * @param vol - the CollisionVolume this collider uses
     * @param isTrigger - true if the volume should ignore physics
     *        and just fire collision events, false otherwise.
     */
    CCollider(std::shared_ptr<GameObject> parent,
              std::shared_ptr<CollisionVolume> vol,
              bool isTrigger);

    /**
     * @brief CCollider constructs a collider component
     * @param parent - the GameObject this collider belongs to
     * @param vol - the CollisionVolume this collider uses
     * @param isTrigger - true if the volume should ignore physics
     *        and just fire collision events, false otherwise.
     * @param resp - the CollisionResponse to execute when this
     *        volume collides with another.
     */
    CCollider(std::shared_ptr<GameObject> parent,
              std::shared_ptr<CollisionVolume> vol,
              bool isTrigger,
              std::shared_ptr<CollisionResponse> resp);

    /**
     * @brief CCollider constructs a collider component
     * @param parent - the GameObject this collider belongs to
     * @param vol - the CollisionVolume this collider uses
     * @param isTrigger - true if the volume should ignore physics
     *        and just fire collision events, false otherwise.
     * @param resp - the CollisionResponse to execute when this
     *        collider collides with another.
     * @param collision_layer - the collision layer this collider
     *        resides on.
     */
    CCollider(std::shared_ptr<GameObject> parent,
              std::shared_ptr<CollisionVolume> vol,
              bool isTrigger,
              std::shared_ptr<CollisionResponse> resp,
              int collision_layer);

    ~CCollider();

    /**
     * @brief getCollisionVolume gets a shared pointer to the
     *        underlying CollisionVolume which gives the Collider
     *        its shape.
     * @return a shared pointer to the Collider's volume
     */
    std::shared_ptr<CollisionVolume> getCollisionVolume();

    /**
     * @brief isTrigger tells whether the Collider is a trigger.
     * @return true if the Collider is a trigger (should ignore
     *         MTV & just generate collision callbacks) or false
     *         if the Collider is a physical collider.
     */
    bool isTrigger() const;

    /**
     * @brief onCollide is called whenever this Collider collides
     *        with another. This is where the callback to the
     *        CollisionResponse is generated.
     * @param other - the owner of the Collider that this Collider
     *        touched.
     */
    void onCollide(std::shared_ptr<GameObject> other);

    /**
     * @brief intersect takes in a ray for raycasting and returns
     *        the t at which the ray intersects with the collider,
     *        or -1 if it does not hit. This is implemented per
     *        CollisionVolume, so we simply pass the call on.
     * @param origin - the origin point of the ray
     * @param direction - the direction of the ray
     * @return the t-value of the ray-intersect equation, or -1 on
     *         a miss.
     */
    float intersect(glm::vec3 origin, glm::vec3 direction);

    /**
     * @brief getLayer gets the Collision Layer of the Collider.
     * @return an integer, usually a member of the
     *         COLLISION_LAYERS enum.
     */
    int getLayer();

    /**
     * @brief ignores tells whether the Collider ignores the given
     *        layer.
     * @param layer - the layer to check
     * @return true if the Collider ignores the layer (should not
     *         collide), false otherwise
     */
    bool ignores(int layer);

    /**
     * @brief ignoreLayer adds the given layer to the Collider's
     *        ignored set, making it not calculate collisions
     *        with any Colliders on that layer.
     * @param layer - the layer to ignore
     */
    void ignoreLayer(int layer);

    /**
     * @brief unignoreLayer removes the given layer from the
     *        Collider's ignored layer set, allowing it to
     *        collide with colliders on that layer.
     * @param layer - the layer to unignore.
     */
    void unignoreLayer(int layer);

private:
    // The collision volume that defines our shape
    std::shared_ptr<CollisionVolume> m_collVol;

    // True if we should ignore MTV, false otherwise
    const bool m_isTrigger;

    // The response we should call if we hit something
    std::shared_ptr<CollisionResponse> m_collResp;

    // Our own layer
    int m_collisionLayer;

    // All the layers we don't collide with
    QSet<int> m_ignoredLayers;
};

#endif // CCOLLIDER_H
