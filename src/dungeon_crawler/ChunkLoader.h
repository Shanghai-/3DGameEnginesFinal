#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include "engine/systems/System.h"
#include "engine/components/CTransform.h"
#include "engine/objectManagement/GameWorld.h"
#include "engine/graphics/Graphics.h"
#include "ChunkFactory.h"
#include "engine/ai/Blackboard.h"

#include <QHash>

class CChunkInfo;

class ChunkLoader : public System
{
public:
    ChunkLoader(int priority, std::shared_ptr<GameWorld> gw,
                std::shared_ptr<Blackboard> board,
                unsigned long initial_seed);
    ~ChunkLoader();

    QString getComponentType() const { return ""; }
    void addComponent(const std::shared_ptr<Component> &c) {}
    void removeComponent(const std::shared_ptr<Component> &c) {}
    void tick(float seconds);

private:
    void constructAIs(QString chunkName, std::shared_ptr<CChunkInfo> info,
                      int r, int c);

    std::shared_ptr<GameWorld> m_gw;
    std::shared_ptr<Blackboard> m_board;
    std::unique_ptr<ChunkFactory> m_factory;
    Graphics *m_graphics;
    std::mt19937 m_rng;

    QHash<QString, std::shared_ptr<GameObject>> m_chunkMap;
    QHash<QString, QSet<std::shared_ptr<GameObject>>> m_chunkAIs;

    int m_curChunkX, m_curChunkZ, m_originChunkX, m_originChunkZ;
};

#endif // CHUNKLOADER_H
