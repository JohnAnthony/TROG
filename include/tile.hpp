#ifndef TILE_H
#define TILE_H

#define EMPTY_CHAR          ' '
#define FLOOR_CHAR          '.'
#define WALL_CHAR           '#'
#define PILLAR_CHAR         'O'
#define CLOSED_DOOR_CHAR    '+'
#define OPEN_DOOR_CHAR      '/'

class Tile {
    public:
    Tile(void);
    Tile(bool vis, char inC);
    bool isSeeThrough(void);
    bool isPassable(void);
    char getTileType(void);
    void setTileType(char in); 

    bool isVisible;

    private:
    char c;
};

#endif
