#include "SpacePartMap.h"

#include <iostream>
#include <QStack>

SpacePartMap::SpacePartMap(int edge_padding, unsigned long initial_seed)
{
    int width = 25;
    int height = 25;

    // Clamp padding between 1 and 1/2 map size.
    int pad = std::max(1, std::min(edge_padding, (std::max(width, height) / 2) - 1));

    m_padding = pad;
    m_width = std::max(pad, width);
    m_height = std::max(pad, height);
    m_min_room_size = m_padding * MIN_ROOM_PAD_MUL;

    m_rng_engine.seed(initial_seed);

    for (int r = 0; r <= m_height; r++) {
        for (int c = 0; c <= m_width; c++) {
            map_cell *n = &m_map[r][c];
            n->walls[NORTH] = false;
            n->walls[EAST] = false;
            n->walls[SOUTH] = false;
            n->walls[WEST] = false;
            n->isHall = false;
            n->node = nullptr;

            // Literally edge cases
            if (r == 0) n->walls[NORTH] = true;
            if (c == 0) n->walls[WEST] = true;
            if (c == m_width - 1) n->walls[EAST] = true;
            if (r == m_height - 1) n->walls[SOUTH] = true;

            if (c == m_width || r == m_height) {
                n->walls[NORTH] = true;
                n->walls[EAST] = true;
                n->walls[SOUTH] = true;
                n->walls[WEST] = true;
            }
        }
    }

    //m_root = SplitHorizontal(0, m_width - 1, 0, m_height - 1, 0);
    //m_root->parent = nullptr;
    SplitHorizontal(0, m_width - 1, 0, m_height - 1, 0);

    MakeChunkConnectors();
}

SpacePartMap::~SpacePartMap()
{
    int sz = m_rooms.size();
    for (int i = 0; i < sz; i++) {
        delete m_rooms[i];
    }
}

std::vector<SpacePartMap::map_room *> SpacePartMap::getRooms()
{
    return m_rooms;
}

SpacePartMap::map_cell *SpacePartMap::getCell(int r, int c)
{
    return &m_map[r][c];
}

/*
std::vector<SpacePartMap::map_leaf *> SpacePartMap::getRooms()
{
    std::vector<map_leaf *> to_return;

    QStack<map_node *> to_explore;
    to_explore.push(m_root);

    while (!to_explore.empty()) {
        map_node *curr = to_explore.pop();

        if (curr->isLeaf) {
            map_leaf *room = static_cast<map_leaf *>(curr);
            to_return.push_back(room);
        } else {
            to_explore.push(curr->right_child);
            to_explore.push(curr->left_child);
        }
    }

    return to_return;
} */

void SpacePartMap::SplitHorizontal(unsigned int rowStart, unsigned int rowEnd,
                                                      unsigned int colStart, unsigned int colEnd,
                                                      unsigned int depth)
{
    //If the room is very small, we return it as-is
    if ((rowEnd - rowStart) <= m_min_room_size)
    {
        m_rooms.push_back(new map_room(rowStart, rowEnd, colStart, colEnd, depth));
        return;
    }

    std::uniform_int_distribution<int> early_term_dist(0, EARLY_TERMINATION_CHANCE);
    if (early_term_dist(m_rng_engine) == 0 && depth >= MIN_EARLY_TERM_DEPTH)
    {
        m_rooms.push_back(new map_room(rowStart, rowEnd, colStart, colEnd, depth));

        /* int width = colEnd - colStart;
        int height = rowEnd - rowStart;

        int upperBoundRowChange = width / 2 - m_min_room_size / 2;
        if (upperBoundRowChange <= 0) return;

        int upperBoundColChange = height / 2 - m_min_room_size / 2;
        if (upperBoundColChange <= 0) return;

        std::uniform_int_distribution<int> row_dist(0, upperBoundRowChange);
        int newRowStart = rowStart + row_dist(m_rng_engine);
        int newRowEnd = rowEnd - row_dist(m_rng_engine);

        std::uniform_int_distribution<int> col_dist(0, upperBoundColChange);
        int newColStart = colStart + col_dist(m_rng_engine);
        int newColEnd = colEnd - col_dist(m_rng_engine);

        for (int r = rowStart; r <= rowEnd; r++) {
            for (int c = colStart; c <= colEnd; c++) {
                map_cell *n = &m_map[r][c];

                if (r >= newRowStart && r <= newRowEnd && c >= newColStart && c <= newColEnd) {

                    if (r == newRowStart) n->walls[NORTH] = true;
                    if (c == newColStart) n->walls[WEST] = true;
                    if (c == newColEnd) n->walls[EAST] = true;
                    if (r == newRowEnd) n->walls[SOUTH] = true;

                } else {
                    n->walls[NORTH] = true;
                    n->walls[SOUTH] = true;
                    n->walls[EAST] = true;
                    n->walls[WEST] = true;
                }
            }
        } */

        return;
    }

    std::uniform_int_distribution<int> distribution(rowStart + m_padding, rowEnd - m_padding);
    int split = distribution(m_rng_engine);

    for (int c = colStart; c <= colEnd; c++) {
        m_map[split - 1][c].walls[SOUTH] = true;
        m_map[split + 1][c].walls[NORTH] = true;

        map_cell *border = &m_map[split][c];
        border->walls[NORTH] = true;
        border->walls[SOUTH] = true;
        border->walls[EAST] = true;
        border->walls[WEST] = true;
    }

    SplitVertical(rowStart, split - 1, colStart, colEnd, depth + 1);
    SplitVertical(split + 1, rowEnd, colStart, colEnd, depth + 1);

    std::uniform_int_distribution<int> hall(colStart, colEnd);
    int hallCol = hall(m_rng_engine);
    int hallRow = split;
    map_cell *h = &m_map[hallRow][hallCol];

    while (h->walls[NORTH] && !h->isHall) {
        h->walls[NORTH] = false;
        h->walls[SOUTH] = false;
        h->isHall = true;
        hallRow--;
        h = &m_map[hallRow][hallCol];
    }

    h->walls[SOUTH] = false;

    hallRow = split + 1;
    h = &m_map[hallRow][hallCol];

    while (h->walls[SOUTH] && !h->isHall) {
        h->walls[NORTH] = false;
        h->walls[SOUTH] = false;
        h->isHall = true;
        hallRow++;
        h = &m_map[hallRow][hallCol];
    }

    h->walls[NORTH] = false;

    /*
    if (rowEnd - rowStart <= m_padding * MIN_ROOM_PAD_MUL)
    {
        std::uniform_int_distribution<int> height_dist(2, MAX_CEIL_HEIGHT);

        map_leaf *room = new map_leaf;
        room->isLeaf = true;
        room->width = colEnd - colStart + 1;
        room->height = rowEnd - rowStart + 1;
        room->ceiling_height = height_dist(m_rng_engine);
        room->row = rowStart;
        room->col = colStart;
        room->left_child = nullptr;
        room->right_child = nullptr;

        return room;
    }

    std::uniform_int_distribution<int> early_term_dist(0, EARLY_TERMINATION_CHANCE);
    if (early_term_dist(m_rng_engine) == 0 && depth > 0) {

        std::uniform_int_distribution<int> height_dist(2, MAX_CEIL_HEIGHT);
        std::uniform_int_distribution<int> row_start_dist(rowStart, rowStart + (rowEnd - rowStart) / 2);
        std::uniform_int_distribution<int> col_start_dist(colStart, colStart + (colEnd - colStart) / 2);

        rowStart = row_start_dist(m_rng_engine);
        colStart = col_start_dist(m_rng_engine);

        map_leaf *room = new map_leaf;
        room->isLeaf = true;
        room->width = colEnd - colStart + 1;
        room->height = rowEnd - rowStart + 1;
        room->ceiling_height = height_dist(m_rng_engine);
        room->row = rowStart;
        room->col = colStart;
        room->left_child = nullptr;
        room->right_child = nullptr;

        return room;
    }

    std::uniform_int_distribution<int> distribution(rowStart + m_padding, rowEnd - m_padding);
    int split = distribution(m_rng_engine);

    map_node *piece = new map_node;
    piece->isLeaf = false;

    piece->left_child = SplitVertical(rowStart, split - 1, colStart, colEnd, depth + 1);
    piece->left_child->parent = piece;

    piece->right_child = SplitVertical(split + 1, rowEnd, colStart, colEnd, depth + 1);
    piece->right_child->parent = piece;

    return piece; */
}

void SpacePartMap::SplitVertical(unsigned int rowStart, unsigned int rowEnd,
                                                    unsigned int colStart, unsigned int colEnd,
                                                    unsigned int depth)
{
    //If the room is very small, we return it as-is
    if ((colEnd - colStart) <= m_min_room_size) {
        m_rooms.push_back(new map_room(rowStart, rowEnd, colStart, colEnd, depth));
        return;
    }

    std::uniform_int_distribution<int> early_term_dist(0, EARLY_TERMINATION_CHANCE);
    if (early_term_dist(m_rng_engine) == 0 && depth >= MIN_EARLY_TERM_DEPTH)
    {
        m_rooms.push_back(new map_room(rowStart, rowEnd, colStart, colEnd, depth));
        /* int width = colEnd - colStart;
        int height = rowEnd - rowStart;

        int upperBoundRowChange = width / 2 - m_min_room_size / 2;
        if (upperBoundRowChange <= 0) return;

        int upperBoundColChange = height / 2 - m_min_room_size / 2;
        if (upperBoundColChange <= 0) return;

        std::uniform_int_distribution<int> row_dist(0, upperBoundRowChange);
        int newRowStart = rowStart + row_dist(m_rng_engine);
        int newRowEnd = rowEnd - row_dist(m_rng_engine);

        std::uniform_int_distribution<int> col_dist(0, upperBoundColChange);
        int newColStart = colStart + col_dist(m_rng_engine);
        int newColEnd = colEnd - col_dist(m_rng_engine);

        for (int r = rowStart; r <= rowEnd; r++) {
            for (int c = colStart; c <= colEnd; c++) {
                map_cell *n = &m_map[r][c];

                if (r >= newRowStart && r <= newRowEnd && c >= newColStart && c <= newColEnd) {

                    if (r == newRowStart) n->walls[NORTH] = true;
                    if (c == newColStart) n->walls[WEST] = true;
                    if (c == newColEnd) n->walls[EAST] = true;
                    if (r == newRowEnd) n->walls[SOUTH] = true;

                } else {
                    n->walls[NORTH] = true;
                    n->walls[SOUTH] = true;
                    n->walls[EAST] = true;
                    n->walls[WEST] = true;
                }
            }
        } */

        return;
    }

    std::uniform_int_distribution<int> distribution(colStart + m_padding, colEnd - m_padding);
    int split = distribution(m_rng_engine);

    for (int r = rowStart; r <= rowEnd; r++) {
        m_map[r][split - 1].walls[EAST] = true;
        m_map[r][split + 1].walls[WEST] = true;

        map_cell *border = &m_map[r][split];
        border->walls[NORTH] = true;
        border->walls[SOUTH] = true;
        border->walls[EAST] = true;
        border->walls[WEST] = true;
    }

    SplitHorizontal(rowStart, rowEnd, colStart, split - 1, depth + 1);
    SplitHorizontal(rowStart, rowEnd, split + 1, colEnd, depth + 1);

    std::uniform_int_distribution<int> hall(rowStart, rowEnd);
    int hallRow = hall(m_rng_engine);
    int hallCol = split;
    map_cell *h = &m_map[hallRow][hallCol];

    while (h->walls[WEST] && !h->isHall) {
        h->walls[EAST] = false;
        h->walls[WEST] = false;
        h->isHall = true;
        hallCol--;
        h = &m_map[hallRow][hallCol];
    }

    h->walls[EAST] = false;

    hallCol = split + 1;
    h = &m_map[hallRow][hallCol];

    while (h->walls[EAST] && !h->isHall) {
        h->walls[EAST] = false;
        h->walls[WEST] = false;
        h->isHall = true;
        hallCol++;
        h = &m_map[hallRow][hallCol];
    }

    h->walls[WEST] = false;

    /*
    if (colEnd - colStart <= m_padding * MIN_ROOM_PAD_MUL) {

        std::uniform_int_distribution<int> height_dist(2, MAX_CEIL_HEIGHT);

        map_leaf *room = new map_leaf;
        room->isLeaf = true;
        room->width = colEnd - colStart + 1;
        room->height = rowEnd - rowStart + 1;
        room->ceiling_height = height_dist(m_rng_engine);
        room->row = rowStart;
        room->col = colStart;
        room->left_child = nullptr;
        room->right_child = nullptr;

        return room;
    }

    std::uniform_int_distribution<int> early_term_dist(0, EARLY_TERMINATION_CHANCE);
    if (early_term_dist(m_rng_engine) == 0 && depth > 0) {

        std::uniform_int_distribution<int> height_dist(2, MAX_CEIL_HEIGHT);
        std::uniform_int_distribution<int> row_start_dist(rowStart, rowStart + (rowEnd - rowStart) / 2);
        std::uniform_int_distribution<int> col_start_dist(colStart, colStart + (colEnd - colStart) / 2);

        rowStart = row_start_dist(m_rng_engine);
        colStart = col_start_dist(m_rng_engine);

        map_leaf *room = new map_leaf;
        room->isLeaf = true;
        room->width = colEnd - colStart + 1;
        room->height = rowEnd - rowStart + 1;
        room->ceiling_height = height_dist(m_rng_engine);
        room->row = rowStart;
        room->col = colStart;
        room->left_child = nullptr;
        room->right_child = nullptr;

        return room;
    }

    std::uniform_int_distribution<int> distribution(colStart + m_padding, colEnd - m_padding);
    int split = distribution(m_rng_engine);

    map_node *piece = new map_node;
    piece->isLeaf = false;

    piece->left_child = SplitHorizontal(rowStart, rowEnd, colStart, split - 1, depth + 1);
    piece->left_child->parent = piece;

    piece->right_child = SplitHorizontal(rowStart, rowEnd, split + 1, colEnd, depth + 1);
    piece->right_child->parent = piece;

    return piece;
    */
}

void SpacePartMap::MakeChunkConnectors()
{
    // North entrance
    int hallRow = 0;
    int hallCol = m_width / 2;

    map_cell *c = &m_map[hallRow][hallCol];
    while (c->walls[SOUTH] && !c->isHall) {
        c->walls[NORTH] = false;
        c->walls[SOUTH] = false;
        c->isHall = true;
        hallRow++;
        c = &m_map[hallRow][hallCol];
    }
    c->walls[NORTH] = false;

    // West entrance
    hallRow = m_height / 2;
    hallCol = 0;

    c = &m_map[hallRow][hallCol];
    while (c->walls[EAST] && !c->isHall) {
        c->walls[EAST] = false;
        c->walls[WEST] = false;
        c->isHall = true;
        hallCol++;
        c = &m_map[hallRow][hallCol];
    }
    c->walls[WEST] = false;

    // South entrance
    hallRow = m_height;
    hallCol = m_width / 2;

    c = &m_map[hallRow][hallCol];
    while (c->walls[NORTH] && !c->isHall) {
        c->walls[NORTH] = false;
        c->walls[SOUTH] = false;
        c->isHall = true;
        hallRow--;
        c = &m_map[hallRow][hallCol];
    }
    c->walls[SOUTH] = false;

    // East entrance
    hallRow = m_height / 2;
    hallCol = m_width;

    c = &m_map[hallRow][hallCol];
    while (c->walls[WEST] && !c->isHall) {
        c->walls[WEST] = false;
        c->walls[EAST] = false;
        c->isHall = true;
        hallCol--;
        c = &m_map[hallRow][hallCol];
    }
    c->walls[EAST] = false;
}

SpacePartMap::map_room *SpacePartMap::getRoom(int r, int c)
{
    for (int i = 0; i < m_rooms.size(); i++) {
        map_room *rm = m_rooms[i];
        if (rm->start_col <= c && rm->end_col >= c
                && rm->start_row <= r && rm->end_row >= r) return rm;
    }
    return nullptr;
}

// Debug method
void SpacePartMap::print()
{
    for (int r = 0; r <= m_height; r++) {
        for (int c = 0; c <= m_width; c++) {
            map_cell n = m_map[r][c];
            if (n.walls[NORTH] && n.walls[SOUTH] && n.walls[EAST] && n.walls[WEST]) {
                std::cout << "x";
            } else if (n.isHall && (n.walls[NORTH] || n.walls[SOUTH])) {
                std::cout << "═";
            } else if (n.isHall && (n.walls[EAST] || n.walls[WEST])) {
                std::cout << "║";
            } else if (n.walls[NORTH] && n.walls[WEST]) {
                std::cout << "┌";
            } else if (n.walls[NORTH] && n.walls[EAST]) {
                std::cout << "┐";
            } else if (n.walls[SOUTH] && n.walls[WEST]) {
                std::cout << "└";
            } else if (n.walls[SOUTH] && n.walls[EAST]) {
                std::cout << "┘";
            } else if (n.walls[NORTH]) {
                std::cout << "─";
            } else if (n.walls[SOUTH]) {
                std::cout << "─";
            } else if (n.walls[EAST] || n.walls[WEST]) {
                std::cout << "│";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }

    /*
    int map[m_height][m_width];
    for (int r = 0; r < m_height; r++) {
        for (int c = 0; c < m_width; c++) {
            map[r][c] = 0;
        }
    }

    QStack<map_node *> to_explore;
    to_explore.push(m_root);

    while (!to_explore.empty()) {
        map_node *curr = to_explore.pop();

        if (curr->isLeaf) {
            map_leaf *room = static_cast<map_leaf *>(curr);
            for (int r = room->row; r < room->row + room->height; r++) {
                for (int c = room->col; c < room->col + room->width; c++) {
                    map[r][c] = room->ceiling_height;
                }
            }

            // TODO: this won"t work on a chunk that"s just one giant room (root node)
            map_node *right = room->parent->right_child;
            map_node *left = room->parent->left_child;
            map_node *sibling = (right == room) ? left : right;

            if (sibling->isLeaf) {
                map_leaf *to_connect = static_cast<map_leaf *>(sibling);
                map[to_connect->row][to_connect->col] = 8;
            }
        } else {
            to_explore.push(curr->right_child);
            to_explore.push(curr->left_child);
        }
    }

    for (int r = 0; r < m_height; r++) {
        for (int c = 0; c < m_width; c++) {
            std::cout << map[r][c];
        }
        std::cout << std::endl;
    } */
}
