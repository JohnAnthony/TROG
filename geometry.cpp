#include "geometry.hpp"
#include <ncurses.h>

#define ABS(x)  ((x < 0) ? -x : x)

// extern std::string status_line;
// 
bool
Point::operator==(Point const& other) {
    return this->x == other.x && this->y == other.y;
}

float
CalculateDistance(Point p1, Point p2) {
    Point p3;
    float ret;

    p3.x = ABS(p1.x - p2.x);
    p3.y = ABS(p1.y - p2.y);

    ret = MAX(p3.x, p3.y) * 1.41;
    ret += MIN(p3.x, p3.y);

    return ret;
}

