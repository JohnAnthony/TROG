#ifndef TILE_H
#define TILE_H

#define EMPTY_CHAR          ' '
#define FLOOR_CHAR          '.'
#define WALL_CHAR           '#'
#define CLOSED_DOOR_CHAR    '+'
#define OPEN_DOOR_CHAR      '/'

class Tile {
    public:
    Tile(void);
    Tile(bool vis, char inC);
    bool SeeThrough(void);

    char c;
    bool isVisible;
};

#endif
