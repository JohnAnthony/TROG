#ifndef LEVEL_H
#define LEVEL_H

#include "tile.hpp"
#include "geometry.hpp"

#define MAP_W   320
#define MAP_H   320

#define EMPTY_CHAR          ' '
#define FLOOR_CHAR          '.'
#define WALL_CHAR           '#'
#define CLOSED_DOOR_CHAR    'O'
#define OPEN_DOOR_CHAR      '/'

typedef Rect Room;
typedef Rect Corridor;

class Level {
    public:
    Level(Level* parent);
    ~Level(void);

    int             depth;
    Tile            tiles[MAP_W][MAP_H];
    Level*          prev;
    Level*          next;

    private:
    void ApplyRoom(Room *r);
    void ApplyCorridor(Corridor *c);
};

#endif
