#ifndef CCHUNKINFO_H
#define CCHUNKINFO_H

#include "engine/components/Component.h"
#include "engine/gen/SpacePartMap.h"
#include "glm/glm.hpp"

class CChunkInfo : public Component
{
public:
    CChunkInfo(std::shared_ptr<GameObject> parent, std::vector<SpacePartMap::map_room *> rooms);
    ~CChunkInfo();

    std::vector<SpacePartMap::map_room> *getRoomVector();

    int getRoom(glm::vec3 worldPos);

private:
    std::vector<SpacePartMap::map_room> m_rooms;
};

#endif // CCHUNKINFO_H
