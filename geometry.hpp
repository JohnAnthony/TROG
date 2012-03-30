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
        WEST
    } Type;
}

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

struct Point {
    int x;
    int y;
};

void ScreenNoise(void);
bool IsOnScreen(Point p);
void Alert(std::string str);
bool BinaryChoice(std::string str, char a, char b);

#endif
