#include "CChunkInfo.h"

#include <iostream>
#include "glm/gtx/string_cast.hpp"

CChunkInfo::CChunkInfo(std::shared_ptr<GameObject> parent, std::vector<SpacePartMap::map_room *> rooms) :
    Component(parent)
{
    for (int i = 0; i < rooms.size(); i++) {
        m_rooms.push_back(*rooms[i]);
    }
}

CChunkInfo::~CChunkInfo()
{
}

std::vector<SpacePartMap::map_room> *CChunkInfo::getRoomVector()
{
    return &m_rooms;
}

int CChunkInfo::getRoom(glm::vec3 worldPos) {
    std::cout << "World pos is " << glm::to_string(worldPos) << std::endl;
    glm::vec3 chunkPos = glm::mod(worldPos, 13.0f);
    std::cout << "Chunk pos is " << glm::to_string(chunkPos) << std::endl;
    int col = glm::round(chunkPos.x) + 13;
    int row = glm::round(chunkPos.z) + 13;
    std::cout << "row is " << row << std::endl;
    std::cout << "col is " << col << std::endl;

    for (int i = 0; i < m_rooms.size(); i++) {
        SpacePartMap::map_room r = m_rooms[i];
        if (r.start_row <= row && r.end_row >= row &&
                r.start_col <= col && r.end_col >= col) {
            std::cout << "FOUND ROOM: " << i << std::endl;
            return i;
        }
    }

    return -1;
}
