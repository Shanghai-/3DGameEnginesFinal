#include "NavmeshDrawer.h"

#include "engine/graphics/Graphics.h"
#include "engine/ai/AStarNode.h"

NavmeshDrawer::NavmeshDrawer(int priority) :
    System(priority)
{
    m_graphics = Graphics::getGlobalInstance();
}

NavmeshDrawer::~NavmeshDrawer()
{
}

QString NavmeshDrawer::getComponentType() const
{
    return typeid(CNavMesh).name();
}

inline uint qHash(const std::shared_ptr<CNavMesh> &key)
{
    return key->getId();
}

void NavmeshDrawer::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CNavMesh> nav = std::dynamic_pointer_cast<CNavMesh>(c);
    m_meshes.insert(nav);
}

void NavmeshDrawer::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CNavMesh> nav = std::dynamic_pointer_cast<CNavMesh>(c);
    m_meshes.remove(nav);
}

void NavmeshDrawer::draw()
{
    QSetIterator<std::shared_ptr<CNavMesh>> it(m_meshes);
    while (it.hasNext()) {
        QSetIterator<AStarNode *> inner(it.next()->getNodes());
        while (inner.hasNext()) {
            AStarNode *curr = inner.next();

            m_graphics->clearTransform();
            m_graphics->translate(curr->getPosition());
            m_graphics->scale(glm::vec3(0.1f));
            if (curr->onPath) {
                m_graphics->setMaterial("red");
            } else {
                m_graphics->setMaterial("yellow");
            }
            m_graphics->drawShape("cube");

            /*QVectorIterator<AStarNode *> edges(curr->getEdges());
            while (edges.hasNext()) {
                AStarNode *other = edges.next();

                m_graphics->clearTransform();
                m_graphics->translate((curr->getPosition() + other->getPosition()) / glm::vec3(2.0f));
                m_graphics->scale(glm::vec3(glm::max(curr->getPosition().x - other->getPosition().x, 0.1f),
                                            0.1f,
                                            glm::max(curr->getPosition().z - other->getPosition().z, 0.1f)));

                if (curr->onPath && other->onPath) {
                    m_graphics->setMaterial("red");
                } else {
                    m_graphics->setMaterial("yellow");
                }

                m_graphics->drawShape("wire_cube");
            }*/
        }
    }
}
