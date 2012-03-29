#ifndef LEVEL_H
#define LEVEL_H

#include "tile.hpp"

#define MAP_W   320
#define MAP_H   320

class Level {
    public:
    Level(Level* parent);
    ~Level(void);

    int             depth;
    Tile            tiles[MAP_W][MAP_H];
    Level*          prev;
    Level*          next;
};

#endif
