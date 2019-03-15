#ifndef SPACEPARTMAP_H
#define SPACEPARTMAP_H

#include <memory>
#include <random>

#define EARLY_TERMINATION_CHANCE 4 // 1 over this number = term chance
#define MIN_EARLY_TERM_DEPTH 2
#define MIN_CEIL_HEIGHT 2
#define MAX_CEIL_HEIGHT 4 // Rooms can be between 2 and this number units tall
#define MIN_ROOM_PAD_MUL 2 // Forbid rooms smaller than this number times pad size

class AStarNode;

class SpacePartMap
{
public:
    enum DIRECTIONS {
        NORTH = 0,
        EAST = 1,
        SOUTH = 2,
        WEST = 3
    };

    /* struct map_node {
        bool isLeaf;
        map_node *parent;
        map_node *left_child;
        map_node *right_child;
    };

    struct map_leaf : map_node {
        int width, height, ceiling_height;
        int row, col;
    }; */

    struct map_room {
        map_room(int start_r, int end_r, int start_c, int end_c, int depth) :
            start_row(start_r), end_row(end_r),
            start_col(start_c), end_col(end_c),
            depth(depth)
        {}

        int start_row, end_row;
        int start_col, end_col;
        int depth;
        //std::vector<AStarNode *> nodes;
    };

    struct map_cell {
        bool walls[4];
        bool isHall;
        AStarNode *node;
    };

    SpacePartMap(int edge_padding,
                 unsigned long initial_seed);
    ~SpacePartMap();

    std::vector<map_room *> getRooms();
    map_room *getRoom(int r, int c);
    map_cell *getCell(int r, int c);

    void print();

private:
    void SplitHorizontal(unsigned int rowStart, unsigned int rowEnd,
                              unsigned int colStart, unsigned int colEnd,
                              unsigned int depth);
    void SplitVertical(unsigned int rowStart, unsigned int rowEnd,
                            unsigned int colStart, unsigned int colEnd,
                            unsigned int depth);

    void MakeChunkConnectors();

    //std::vector<std::vector<map_cell *>> m_map;
    map_cell m_map[26][26];
    std::vector<map_room *> m_rooms;

    unsigned int m_padding;
    unsigned int m_width, m_height;
    unsigned int m_min_room_size;
    std::mt19937 m_rng_engine;
};

#endif // SPACEPARTMAP_H
