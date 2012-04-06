#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>

#define NOISE_CHARACTER  '#'

#define MAX(x, y) ((x > y) ? x : y )
#define MIN(x, y) ((x < y) ? x : y )

namespace Direction {
    typedef enum {
        NORTH,
        SOUTH,
        EAST,
        WEST,
        NW,
        NE,
        SW,
        SE
    } Type;
}

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

class Point {
    public:
    bool operator==(Point const& other);
    int x;
    int y;
};

namespace CustColour {
    typedef enum {
        WHITE = 0,
        RED = 1,
        YELLOW = 2,
        GREEN = 3,
        BLUE = 4
    } Type;
}

void ScreenNoise(void);
bool IsOnScreen(Point p);
void Alert(std::string str);
bool BinaryChoice(std::string str, char a, char b);

#endif
