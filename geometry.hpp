#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>

#define MAX(x, y)   ((x > y) ? x : y )
#define MIN(x, y)   ((x < y) ? x : y )
#define LENGTH(X)   (sizeof X / sizeof X[0])
#define ABS(x)  ((x < 0) ? -x : x)
#define RANDOM_IN_RANGE(min, max)   ((rand() % (max - min)) + min)

namespace Direction {
    typedef enum {
        NORTH,
        SOUTH,
        EAST,
        WEST,
        NW,
        NE,
        SW,
        SE,
        LAST_DIRECTION
    } Type;
}

typedef enum {
    HEAD,
    BODY,
    WEAPON,
    SHIELD,
    GLOVES,
    BOOTS,
    RING1,
    RING2,
    NECK,
    LAST_EQUIP_LOCATION
} EquipLocations;

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

struct Range {
    int start;
    int end;
};

class Point {
    public:
    bool operator==(Point const& other);
    int x;
    int y;
};

enum Color {
    COL_WHITE = 0,
    COL_RED = 1,
    COL_GREEN = 2,
    COL_YELLOW = 3,
    COL_BLUE = 4,
    COL_PURPLE = 5,
    COL_CYAN = 6,
    COL_GREY = 8,
    COL_GREYWHITE = 15
};

bool IsOnScreen(Point p);
void Alert(std::string str);
bool BinaryChoice(std::string str, char a, char b);
float CalculateDistance(Point p1, Point p2);
Direction::Type MoveCardinal(Direction::Type turn_to, Direction::Type d);
Point GetRelativePoint(Direction::Type d, Point p);
int DICEROLL(int num, int sz);

#endif
