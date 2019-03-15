#ifndef CHUNKFACTORY_H
#define CHUNKFACTORY_H

#include "engine/objectManagement/GameObject.h"
#include "engine/gen/SpacePartMap.h"
#include "engine/graphics/Graphics.h"

// position vec3, norm vec3, texCoords vec2 = 8 values total
#define VERTEX_LENGTH 8

// Size of one map square in world units
#define SQUARE_SIZE 1.0f

#define TEX_SQUARE_PIX 64.0f
#define TEX_WIDTH 3

class ChunkFactory
{
public:
    ChunkFactory(unsigned long base_seed);
    ~ChunkFactory();

    std::shared_ptr<GameObject> generateChunk(int x, int z, unsigned int id);

private:
    enum geometry_type {
        FLOOR, WALL, CEILING
    };

    //AStarNode *drawFromGrid(SpacePartMap *m, std::vector<float> *points);
    void generate(SpacePartMap *m, std::shared_ptr<GameObject> o, int x, int z);
    void buildWall(glm::vec3 relativeOrigin, glm::vec3 ascent, glm::vec3 normal,
                   int height, std::vector<float> *points);
    bool isFilled(SpacePartMap::map_cell *c);

    void collidersFromGrid(SpacePartMap *m, std::shared_ptr<GameObject> chonk);

    void AddVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, std::vector<float> *points);
    glm::vec2 GetTextureCoord(geometry_type t);
    unsigned long chunkCoordToSeed(int x, int z);

    unsigned long m_seed;
    std::mt19937 m_rng_engine;

    Graphics *m_graphics;

    float m_tex_square_uv;
    int m_vert_count;
    glm::vec2 m_last_tex;
};

#endif // CHUNKFACTORY_H
