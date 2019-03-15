#include "ChunkLoader.h"

#include <iostream>
#include "engine/components/CNavMesh.h"
#include "engine/components/AIComponent.h"
#include "engine/components/CTransform.h"
#include "engine/components/CCollider.h"
#include "engine/components/volumes/CollBox.h"
#include "engine/components/volumes/CollCylinder.h"
#include "engine/components/CRenderable.h"
#include "CChunkInfo.h"
#include "CHealth.h"
#include "CCooldown.h"

inline uint qHash(const std::shared_ptr<GameObject> &key)
{
    return key->getID();
}

ChunkLoader::ChunkLoader(int priority,
                         std::shared_ptr<GameWorld> gw,
                         std::shared_ptr<Blackboard> board,
                         unsigned long initial_seed) :
    System(priority),
    m_gw(gw),
    m_board(board),
    m_originChunkX(0),
    m_originChunkZ(0)
{
    m_factory = std::make_unique<ChunkFactory>(initial_seed);
    m_graphics = Graphics::getGlobalInstance();

    for (int r = -2; r <= 2; r++) {
        for (int c = -2; c <= 2; c++) {
            if (glm::abs(r) + glm::abs(c) <= 2) {
                QString chunkName = QStringLiteral("chunk(%1,%2)").arg(c).arg(r);

                auto chonk = m_factory->generateChunk(c,r,m_gw->getNewObjID());
                m_chunkMap.insert(chunkName, chonk);
                m_gw->addGameObject(chonk);

                std::shared_ptr<CChunkInfo> info = chonk->getComponent<CChunkInfo>();
                constructAIs(chunkName, info, r, c);
            }
        }
    }
}

ChunkLoader::~ChunkLoader()
{
}

void ChunkLoader::tick(float seconds)
{
    std::shared_ptr<CTransform> target = m_board->getComponent<CTransform>("player_transform");

    float fixedX = target->pos.x, fixedZ = target->pos.z;

    if (target->pos.x > 0.f) {
        fixedX += 12.5f;
    } else {
        fixedX -= 12.5f;
    }

    if (target->pos.z > 0.f) {
        fixedZ += 12.5f;
    } else {
        fixedZ -= 12.5f;
    }

    int newChunkX = fixedX / 26;
    int newChunkZ = fixedZ / 26;

    int xDist = glm::abs(newChunkX - m_originChunkX);
    int zDist = glm::abs(newChunkZ - m_originChunkZ);

    QString chunkName = QStringLiteral("chunk(%1,%2)").arg(newChunkX).arg(newChunkZ);
    auto chunk = m_chunkMap.value(chunkName);
    m_board->addComponent("navmesh", chunk->getComponent<CNavMesh>());

    if (xDist + zDist >= 2) {
        /*std::cout << "================" << std::endl;
        std::cout << "ORIGINAL POS: (" << m_originChunkX << ", " << m_originChunkZ << ")" << std::endl;
        std::cout << "NEW POS: (" << newChunkX << ", " << newChunkZ << ")" << std::endl;
        std::cout << "DISTANCE: (" << xDist << ", " << zDist << ")" << std::endl;
        std::cout << "ACTUAL POS: (" << target->pos.x << ", " << target->pos.z << ")" << std::endl; */

        for (int r = -4; r <= 4; r++) {
            for (int c = -4; c <= 4; c++) {
                int newZ = newChunkZ + r;
                int newX = newChunkX + c;

                QString chunkName = QStringLiteral("chunk(%1,%2)").arg(newX).arg(newZ);

                if (glm::abs(r) + glm::abs(c) <= 2) {

                    // If we don't already have this chunk generated and stored, do that
                    if (!m_chunkMap.contains(chunkName)) {
                        auto chonk = m_factory->generateChunk(newX,newZ,m_gw->getNewObjID());

                        m_chunkMap.insert(chunkName, chonk);
                        m_gw->addGameObject(chonk);

                        std::shared_ptr<CChunkInfo> info = chonk->getComponent<CChunkInfo>();
                        constructAIs(chunkName, info, newZ, newX);
                    }
                } else {
                    // If the chunk is out of range but stored in the map, it dies
                    if (m_chunkMap.contains(chunkName)) {
                        auto chonk = m_chunkMap.take(chunkName);

                        //std::cout << "Removing " << chonk->getName().toStdString() << std::endl;
                        m_gw->removeGameObject(chonk);
                        m_graphics->removeShape(chonk->getName().toStdString());

                        QSet<std::shared_ptr<GameObject>> AIs = m_chunkAIs.take(chunkName);
                        QSetIterator<std::shared_ptr<GameObject>> it(AIs);
                        while (it.hasNext()) {
                            m_gw->removeGameObject(it.next());
                        }
                    }
                }
            }
        }

        m_originChunkX = newChunkX;
        m_originChunkZ = newChunkZ;
    }
}

void ChunkLoader::constructAIs(QString chunkName, std::shared_ptr<CChunkInfo> info,
                               int r, int c)
{
    m_rng.seed(qHash(chunkName));
    QSet<std::shared_ptr<GameObject>> AIs;
    //std::shared_ptr<CChunkInfo> info = chonk->getComponent<CChunkInfo>();

    std::vector<SpacePartMap::map_room> *rooms = info->getRoomVector();
    for (int i = 0; i < rooms->size(); i++) {
        SpacePartMap::map_room curr_room = rooms->at(i);
        int width = curr_room.end_col - curr_room.start_col;
        int height = curr_room.end_row - curr_room.start_row;
        int numEnemies = std::max(width, height) / 7;

        //if (numEnemies > 0) {
        for (int i = 0; i < numEnemies; i++) {
            std::uniform_int_distribution<int> row(curr_room.start_row, curr_room.end_row);
            std::uniform_int_distribution<int> col(curr_room.start_col, curr_room.end_col);
            int enemy_row = row(m_rng);
            int enemy_col = col(m_rng);

            glm::vec3 tileOrigin((enemy_col - 12.0f) * SQUARE_SIZE, 0.5f, (enemy_row - 12.0f) * SQUARE_SIZE);
            tileOrigin += glm::vec3(c * 26.0f * SQUARE_SIZE, 0, r * 26.0f * SQUARE_SIZE);

            std::shared_ptr<GameObject> enemy = std::make_shared<GameObject>("Enemy", m_gw->getNewObjID());
            enemy->addComponent(std::make_shared<CTransform>(enemy, false, tileOrigin, glm::vec3(0.0f), glm::vec3(0.4f, 0.85f, 0.4f)));
            enemy->addComponent(std::make_shared<CRenderable>(enemy, "cube"));
            auto vol = std::make_shared<CollBox>(glm::vec3(0.0f), glm::vec3(0.6f, 0.85f, 0.6f));
            std::shared_ptr<CCollider> coll = std::make_shared<CCollider>(enemy, vol, false);
            coll->ignoreLayer(CCollider::WORLD);
            enemy->addComponent(coll);
            enemy->addComponent(std::make_shared<AIComponent>(enemy, m_board));
            enemy->addComponent(std::make_shared<CHealth>(enemy, 2));
            enemy->addComponent(std::make_shared<CCooldown>(enemy, 0.8f));
            m_gw->addGameObject(enemy);

            AIs.insert(enemy);
        }

        if (numEnemies == 0 && curr_room.depth > 2) {
            std::uniform_int_distribution<int> spawn(0, 2);
            if (spawn(m_rng) == 0) {
                float center_x = curr_room.start_col + (curr_room.end_col - curr_room.start_col) / 2.0f;
                float center_z = curr_room.start_row + (curr_room.end_row - curr_room.start_row) / 2.0f;

                glm::vec3 tileOrigin((center_x - 12.0f) * SQUARE_SIZE, 0.5f, (center_z - 12.0f) * SQUARE_SIZE);
                tileOrigin += glm::vec3(c * 26.0f * SQUARE_SIZE, 0, r * 26.0f * SQUARE_SIZE);

                std::shared_ptr<GameObject> item = std::make_shared<GameObject>("Item", m_gw->getNewObjID());
                item->addComponent(std::make_shared<CTransform>(item, true, tileOrigin, glm::vec3(0.0f), glm::vec3(0.005f)));
                item->addComponent(std::make_shared<CRenderable>(item, ":/models/Love.obj", "item"));
                auto coll = std::make_shared<CollCylinder>(glm::vec3(0.0f, -0.5f, 0.0f), 1.0f, 0.5f);
                item->addComponent(std::make_shared<CCollider>(item, coll, true));
                m_gw->addGameObject(item);

                AIs.insert(item);
            }
        }
    }

    m_chunkAIs.insert(chunkName, AIs);
}
