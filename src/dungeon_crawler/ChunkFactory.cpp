#include "ChunkFactory.h"

#include "engine/components/CRenderable.h"
#include "engine/components/CTransform.h"
#include "engine/components/volumes/CollBox.h"
#include "engine/components/volumes/CollComposite.h"
#include "engine/components/CCollider.h"
#include "engine/components/CNavMesh.h"
#include "engine/ai/AStarNode.h"
#include "CChunkInfo.h"

#include <sstream>

ChunkFactory::ChunkFactory(unsigned long base_seed) :
    m_seed(base_seed),
    m_vert_count(-1),
    m_last_tex(glm::vec2(0,0))
{
    m_tex_square_uv = TEX_SQUARE_PIX / (TEX_SQUARE_PIX * TEX_WIDTH);
    m_graphics = Graphics::getGlobalInstance();
}

ChunkFactory::~ChunkFactory()
{
}

std::shared_ptr<GameObject> ChunkFactory::generateChunk(int x, int z, unsigned int id)
{
    //std::cout << "Generating map for (" << x << "," << z << ")" << std::endl;
    std::ostringstream stream;
    stream << "chunk(" << x << "," << z << ")";
    std::string name = stream.str();
    QString qName = QString::fromStdString(name);

    // Use our coordinates along with our base seed to generate a seed
    // for the map generator that always gives the same results for the
    // same chunk coordinate set
    unsigned long seed = chunkCoordToSeed(x,z) + m_seed;
    SpacePartMap *m = new SpacePartMap(3, seed);
    m_rng_engine.seed(seed);



    std::shared_ptr<GameObject> chonk = std::make_shared<GameObject>(qName, id);
    generate(m, chonk, x, z);

    chonk->addComponent(std::make_shared<CChunkInfo>(chonk, m->getRooms()));

    return chonk;
}

//AStarNode *ChunkFactory::drawFromGrid(SpacePartMap *m, std::vector<float> *points)
void ChunkFactory::generate(SpacePartMap *m, std::shared_ptr<GameObject> o, int x, int z)
{
    glm::vec3 centerPoint(x * 26.0f * SQUARE_SIZE, 0, z * 26.0f * SQUARE_SIZE);
    o->addComponent(std::make_shared<CTransform>(o, true, centerPoint));

    // We push back a rough estimate of the vertices we'll need
    std::vector<float> *points = new std::vector<float>();
    points->reserve(25 * 25 * 6 * VERTEX_LENGTH);

    auto composite = std::make_shared<CollComposite>();

    QSet<AStarNode *> nodes;
    SpacePartMap::map_cell *curr;
    for (int r = 0; r <= 25; r++) {
        for (int c = 0; c <= 25; c++) {
            curr = m->getCell(r, c);

            if (isFilled(curr)) {
                glm::vec3 tileOrigin((c - 12.0f) * SQUARE_SIZE, 1.5f * SQUARE_SIZE, (r - 12.0f) * SQUARE_SIZE);

                // If the current cell is totally filled with walls, but one of the adjacent cells isn't, we
                // make the whole cell into one thick collider
                if ((r > 0 && !isFilled(m->getCell(r - 1, c))) ||
                        (c > 0 && !isFilled(m->getCell(r, c - 1))) ||
                        (r < 25 && !isFilled(m->getCell(r + 1, c))) ||
                        (c < 25 && !isFilled(m->getCell(r, c + 1)))) {
                    auto box = std::make_shared<CollBox>(tileOrigin, glm::vec3(SQUARE_SIZE, 6.0f, SQUARE_SIZE));
                    composite->addCollider(box);
                }

                continue;

            } else if (curr->isHall) {
                glm::vec3 tileOrigin((c - 12.5f) * SQUARE_SIZE, 1.5f * SQUARE_SIZE, (r - 12.5f) * SQUARE_SIZE);

                // If the current cell is a hall, and any of its walls border a non-filled space (aka a space
                // that isn't taken care of by the clause above), make a thin collision wall
                if (curr->walls[SpacePartMap::NORTH] && (r > 0 && !isFilled(m->getCell(r - 1, c)))) {
                    auto box = std::make_shared<CollBox>(tileOrigin + glm::vec3(0.5f * SQUARE_SIZE, 0, 0),
                                                         glm::vec3(SQUARE_SIZE, 6.0f, 0.12f));
                    composite->addCollider(box);
                }
                if (curr->walls[SpacePartMap::WEST] && (c > 0 && !isFilled(m->getCell(r, c - 1)))) {
                    auto box = std::make_shared<CollBox>(tileOrigin + glm::vec3(0, 0, 0.5f * SQUARE_SIZE),
                                                         glm::vec3(0.12, 6.0f, SQUARE_SIZE));
                    composite->addCollider(box);
                }
                if (curr->walls[SpacePartMap::SOUTH] && (r < 25 && !isFilled(m->getCell(r + 1, c)))) {
                    auto box = std::make_shared<CollBox>(tileOrigin + glm::vec3(0.5f * SQUARE_SIZE, 0, SQUARE_SIZE),
                                                         glm::vec3(SQUARE_SIZE, 6.0f, 0.12f));
                    composite->addCollider(box);
                }
                if (curr->walls[SpacePartMap::EAST] && (c < 25 && !isFilled(m->getCell(r, c + 1)))) {
                    auto box = std::make_shared<CollBox>(tileOrigin + glm::vec3(SQUARE_SIZE, 0, 0.5f * SQUARE_SIZE),
                                                         glm::vec3(0.12f, 6.0f, SQUARE_SIZE));
                    composite->addCollider(box);
                }
            }

            glm::vec3 tileOrigin((c - 12.5f) * SQUARE_SIZE, 0, (r - 12.5f) * SQUARE_SIZE);
            int height = curr->isHall ? 1 : 3;

            AStarNode *an = new AStarNode(centerPoint + tileOrigin + glm::vec3(SQUARE_SIZE / 2.0f));
            curr->node = an;
            nodes.insert(an);

            if (r > 0) {
                SpacePartMap::map_cell *other = m->getCell(r - 1, c);
                if (other->node != nullptr && !other->walls[SpacePartMap::SOUTH]
                        && !curr->walls[SpacePartMap::NORTH]) {
                    other->node->addEdge(an, true);
                }
            }
            if (c > 0) {
                SpacePartMap::map_cell *other = m->getCell(r, c - 1);
                if (other->node != nullptr && !other->walls[SpacePartMap::EAST]
                        && !curr->walls[SpacePartMap::WEST]) {
                    other->node->addEdge(an, true);
                }
            }

            // NORTH WALL
            if (curr->walls[SpacePartMap::NORTH]) {
                glm::vec3 relativeOrigin = tileOrigin;
                glm::vec3 ascent = relativeOrigin + glm::vec3(SQUARE_SIZE, 0, 0);
                glm::vec3 normal(0.0f, 0.0f, 1.0f);
                buildWall(relativeOrigin, ascent, normal, height, points);
            } else {
                if (r == 0 || (!curr->isHall && m->getCell(r - 1, c)->isHall)) {
                    glm::vec3 relativeOrigin = tileOrigin + glm::vec3(0, SQUARE_SIZE, 0);
                    glm::vec3 ascent = relativeOrigin + glm::vec3(SQUARE_SIZE, 0, 0);
                    glm::vec3 normal(0.0f, 0.0f, 1.0f);
                    buildWall(relativeOrigin, ascent, normal, 2, points);
                }
            }

            // WEST WALL
            if (curr->walls[SpacePartMap::WEST]) {
                glm::vec3 relativeOrigin = tileOrigin + glm::vec3(0, 0, SQUARE_SIZE);
                glm::vec3 ascent = tileOrigin;
                glm::vec3 normal(1.0f, 0.0f, 0.0f);
                buildWall(relativeOrigin, ascent, normal, height, points);
            } else {
                if (c == 0 || (!curr->isHall && m->getCell(r, c - 1)->isHall)) {
                    glm::vec3 relativeOrigin = tileOrigin + glm::vec3(0, SQUARE_SIZE, SQUARE_SIZE);
                    glm::vec3 ascent = tileOrigin + glm::vec3(0, SQUARE_SIZE, 0);
                    glm::vec3 normal(1.0f, 0.0f, 0.0f);
                    buildWall(relativeOrigin, ascent, normal, 2, points);
                }
            }

            // SOUTH WALL
            if (curr->walls[SpacePartMap::SOUTH]) {
                glm::vec3 relativeOrigin = tileOrigin + glm::vec3(SQUARE_SIZE, 0, SQUARE_SIZE);
                glm::vec3 ascent = relativeOrigin - glm::vec3(SQUARE_SIZE, 0, 0);
                glm::vec3 normal(0.0f, 0.0f, -1.0f);
                buildWall(relativeOrigin, ascent, normal, height, points);
            } else {
                if (r == 25 || (!curr->isHall && m->getCell(r + 1, c)->isHall)) {
                    glm::vec3 relativeOrigin = tileOrigin + glm::vec3(SQUARE_SIZE);
                    glm::vec3 ascent = relativeOrigin - glm::vec3(SQUARE_SIZE, 0, 0);
                    glm::vec3 normal(0.0f, 0.0f, -1.0f);
                    buildWall(relativeOrigin, ascent, normal, 2, points);
                }
            }

            // EAST WALL
            if (curr->walls[SpacePartMap::EAST]) {
                glm::vec3 relativeOrigin = tileOrigin + glm::vec3(SQUARE_SIZE, 0, 0);
                glm::vec3 ascent = relativeOrigin + glm::vec3(0, 0, SQUARE_SIZE);
                glm::vec3 normal(-1.0f, 0.0f, 0.0f);
                buildWall(relativeOrigin, ascent, normal, height, points);
            } else {
                if (c == 25 || (!curr->isHall && m->getCell(r, c + 1)->isHall)) {
                    glm::vec3 relativeOrigin = tileOrigin + glm::vec3(SQUARE_SIZE, SQUARE_SIZE, 0);
                    glm::vec3 ascent = relativeOrigin + glm::vec3(0, 0, SQUARE_SIZE);
                    glm::vec3 normal(-1.0f, 0.0f, 0.0f);
                    buildWall(relativeOrigin, ascent, normal, 2, points);
                }
            }

            // FLOOR
            glm::vec3 normal(0.0f, 1.0f, 0.0f);
            AddVertex(tileOrigin, normal, GetTextureCoord(FLOOR), points);
            AddVertex(tileOrigin + glm::vec3(0, 0, SQUARE_SIZE), normal, GetTextureCoord(FLOOR), points);
            AddVertex(tileOrigin + glm::vec3(SQUARE_SIZE, 0, SQUARE_SIZE), normal, GetTextureCoord(FLOOR), points);
            AddVertex(tileOrigin, normal, GetTextureCoord(FLOOR), points);
            AddVertex(tileOrigin + glm::vec3(SQUARE_SIZE, 0, SQUARE_SIZE), normal, GetTextureCoord(FLOOR), points);
            AddVertex(tileOrigin + glm::vec3(SQUARE_SIZE, 0, 0), normal, GetTextureCoord(FLOOR), points);

            // CEILING
            normal = glm::vec3(0.0f, -1.0f, 0.0f);
            AddVertex(tileOrigin + glm::vec3(SQUARE_SIZE, SQUARE_SIZE * height, SQUARE_SIZE), normal, GetTextureCoord(CEILING), points);
            AddVertex(tileOrigin + glm::vec3(0, SQUARE_SIZE * height, SQUARE_SIZE), normal, GetTextureCoord(CEILING), points);
            AddVertex(tileOrigin + glm::vec3(0, SQUARE_SIZE * height, 0), normal, GetTextureCoord(CEILING), points);
            AddVertex(tileOrigin + glm::vec3(SQUARE_SIZE, SQUARE_SIZE * height, 0), normal, GetTextureCoord(CEILING), points);
            AddVertex(tileOrigin + glm::vec3(SQUARE_SIZE, SQUARE_SIZE * height, SQUARE_SIZE), normal, GetTextureCoord(CEILING), points);
            AddVertex(tileOrigin + glm::vec3(0, SQUARE_SIZE * height, 0), normal, GetTextureCoord(CEILING), points);
        }
    }

    m_graphics->addShape(o->getName().toStdString(), *points, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES);
    o->addComponent(std::make_shared<CRenderable>(o, o->getName(), "dungeon"));
    o->addComponent(std::make_shared<CNavMesh>(o, nodes));

    auto collider = std::make_shared<CCollider>(o, composite, false, nullptr, CCollider::WORLD);
    collider->ignoreLayer(CCollider::WORLD);
    o->addComponent(collider);
}

void ChunkFactory::buildWall(glm::vec3 relativeOrigin, glm::vec3 ascent, glm::vec3 normal,
                             int height, std::vector<float> *points)
{
    for (int i = 0; i < height; i++) {
        AddVertex(relativeOrigin + glm::vec3(0, (i+1) * SQUARE_SIZE, 0), normal, GetTextureCoord(WALL), points);
        AddVertex(relativeOrigin + glm::vec3(0, i * SQUARE_SIZE, 0), normal, GetTextureCoord(WALL), points);
        AddVertex(ascent + glm::vec3(0, i * SQUARE_SIZE, 0), normal, GetTextureCoord(WALL), points);

        AddVertex(relativeOrigin + glm::vec3(0, (i+1) * SQUARE_SIZE, 0), normal, GetTextureCoord(WALL), points);
        AddVertex(ascent + glm::vec3(0, i * SQUARE_SIZE, 0), normal, GetTextureCoord(WALL), points);
        AddVertex(ascent + glm::vec3(0, (i+1) * SQUARE_SIZE, 0), normal, GetTextureCoord(WALL), points);
    }
}

void ChunkFactory::collidersFromGrid(SpacePartMap *m, std::shared_ptr<GameObject> chonk)
{
    auto composite = std::make_shared<CollComposite>();

    SpacePartMap::map_cell *curr;
    for (int r = 0; r <= 25; r++) {
        for (int c = 0; c <= 25; c++) {
            curr = m->getCell(r, c);

            if (isFilled(curr)) {
                glm::vec3 tileOrigin((c - 12.0f) * SQUARE_SIZE, 1.5f * SQUARE_SIZE, (r - 12.0f) * SQUARE_SIZE);

                // If the current cell is totally filled with walls, but one of the adjacent cells isn't, we
                // make the whole cell into one thick collider
                if ((r > 0 && !isFilled(m->getCell(r - 1, c))) ||
                        (c > 0 && !isFilled(m->getCell(r, c - 1))) ||
                        (r < 25 && !isFilled(m->getCell(r + 1, c))) ||
                        (c < 25 && !isFilled(m->getCell(r, c + 1)))) {
                    auto box = std::make_shared<CollBox>(tileOrigin, glm::vec3(SQUARE_SIZE, 6.0f, SQUARE_SIZE));
                    composite->addCollider(box);
                }

            } else if (curr->isHall) {
                glm::vec3 tileOrigin((c - 12.5f) * SQUARE_SIZE, 1.5f * SQUARE_SIZE, (r - 12.5f) * SQUARE_SIZE);

                // If the current cell is a hall, and any of its walls border a non-filled space (aka a space
                // that isn't taken care of by the clause above), make a thin collision wall
                if (curr->walls[SpacePartMap::NORTH] && (r > 0 && !isFilled(m->getCell(r - 1, c)))) {
                    auto box = std::make_shared<CollBox>(tileOrigin + glm::vec3(0.5f * SQUARE_SIZE, 0, 0),
                                                         glm::vec3(SQUARE_SIZE, 6.0f, 0.12f));
                    composite->addCollider(box);
                }
                if (curr->walls[SpacePartMap::WEST] && (c > 0 && !isFilled(m->getCell(r, c - 1)))) {
                    auto box = std::make_shared<CollBox>(tileOrigin + glm::vec3(0, 0, 0.5f * SQUARE_SIZE),
                                                         glm::vec3(0.12, 6.0f, SQUARE_SIZE));
                    composite->addCollider(box);
                }
                if (curr->walls[SpacePartMap::SOUTH] && (r < 25 && !isFilled(m->getCell(r + 1, c)))) {
                    auto box = std::make_shared<CollBox>(tileOrigin + glm::vec3(0.5f * SQUARE_SIZE, 0, SQUARE_SIZE),
                                                         glm::vec3(SQUARE_SIZE, 6.0f, 0.12f));
                    composite->addCollider(box);
                }
                if (curr->walls[SpacePartMap::EAST] && (c < 25 && !isFilled(m->getCell(r, c + 1)))) {
                    auto box = std::make_shared<CollBox>(tileOrigin + glm::vec3(SQUARE_SIZE, 0, 0.5f * SQUARE_SIZE),
                                                         glm::vec3(0.12f, 6.0f, SQUARE_SIZE));
                    composite->addCollider(box);
                }
            }
        }
    }

    auto collider = std::make_shared<CCollider>(chonk, composite, false, nullptr, CCollider::WORLD);
    collider->ignoreLayer(CCollider::WORLD);
    chonk->addComponent(collider);
}

bool ChunkFactory::isFilled(SpacePartMap::map_cell *c)
{
    return c->walls[SpacePartMap::NORTH] &&
           c->walls[SpacePartMap::SOUTH] &&
           c->walls[SpacePartMap::EAST] &&
           c->walls[SpacePartMap::WEST];
}

/* void ChunkFactory::navmeshFromGrid(SpacePartMap *m)
{
    SpacePartMap::map_cell *curr;
    for (int r = 0; r <= 25; r++) {
        for (int c = 0; c <= 25; c++) {
            curr = m->getCell(r, c);

        }
    }
} */

void ChunkFactory::AddVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv,
                             std::vector<float> *points)
{
    points->push_back(position.x);
    points->push_back(position.y);
    points->push_back(position.z);
    points->push_back(normal.x);
    points->push_back(normal.y);
    points->push_back(normal.z);
    points->push_back(uv.x);
    points->push_back(uv.y);
}

glm::vec2 ChunkFactory::GetTextureCoord(geometry_type t)
{
    m_vert_count++;

    switch (m_vert_count) {
        case 1:
            return m_last_tex + glm::vec2(m_tex_square_uv, 0.0f);
        case 2:
            return m_last_tex + glm::vec2(m_tex_square_uv);
        case 3:
            return m_last_tex;
        case 4:
            return m_last_tex + glm::vec2(m_tex_square_uv);
        case 5:
            return m_last_tex + glm::vec2(0.0f, m_tex_square_uv);
    }

    std::uniform_int_distribution<int> dist(0, TEX_WIDTH - 1);
    int col = dist(m_rng_engine);
    float colPos = col * m_tex_square_uv;

    float rowPos = 0.0f;
    switch(t) {
        case FLOOR:
            rowPos = m_tex_square_uv * 2;
            break;
        case WALL:
            rowPos = m_tex_square_uv;
            break;
        case CEILING:
            rowPos = 0.0f;
            break;
    }

    m_last_tex = glm::vec2(colPos, rowPos);

    m_vert_count = 0;

    return m_last_tex;
}

unsigned long ChunkFactory::chunkCoordToSeed(int x, int z)
{
    return (44621 + qHash(x)) * 44621 + qHash(z);
}
