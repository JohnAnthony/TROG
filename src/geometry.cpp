#include "geometry.hpp"

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

Point
GetRelativePoint(Direction::Type d, Point p) {
    Point ret;

    if (d == Direction::NW) {
        ret.x = p.x - 1;
        ret.y = p.y - 1;
    }
    else if (d == Direction::NORTH) {
        ret.x = p.x;
        ret.y = p.y - 1;
    }
    else if (d == Direction::NE) {
        ret.x = p.x + 1;
        ret.y = p.y - 1;
    }
    else if (d == Direction::WEST) {
        ret.x = p.x - 1;
        ret.y = p.y;
    }
    else if (d == Direction::EAST) {
        ret.x = p.x + 1;
        ret.y = p.y;
    }
    else if (d == Direction::SW) {
        ret.x = p.x - 1;
        ret.y = p.y + 1;
    }
    else if (d == Direction::SOUTH) {
        ret.x = p.x;
        ret.y = p.y + 1;
    }
    else if (d == Direction::SE) {
        ret.x = p.x + 1;
        ret.y = p.y + 1;
    }
    else // Something went wrong. Just return the original.
        return p;

    return ret;
}

// Function is incomplete
// Currently only handles NORTH and SOUTH
// (because that's all I need at the moment)
Direction::Type
MoveCardinal(Direction::Type turn_to, Direction::Type d) {
    if (d == Direction::LAST_DIRECTION)
        return turn_to;

    if (turn_to == Direction::SOUTH) {
        if      (d == Direction::NW)
            return Direction::WEST;
        else if (d == Direction::NE)
            return Direction::EAST;
        else if (d == Direction::NORTH)
            return Direction::SOUTH;
        else if (d == Direction::WEST)
            return Direction::SW;
        else if (d == Direction::EAST)
            return Direction::SE;
    }
    else if (turn_to == Direction::NORTH) {
        if      (d == Direction::SW)
            return Direction::WEST;
        else if (d == Direction::SE)
            return Direction::EAST;
        else if (d == Direction::SOUTH)
            return Direction::NORTH;
        else if (d == Direction::WEST)
            return Direction::NW;
        else if (d == Direction::EAST)
            return Direction::NE;
    }
    return d;
}

int
DICEROLL(int num, int sz) {
    if (num <= 1)
        return rand() % sz + 1;
    return DICEROLL(num - 1, sz) + rand() % sz + 1;
}

Direction::Type // really crude. Could be much mroe efficient
DirectionFromAToB(Point p1, Point p2) {
    if (p1.x == p2.x && p2.y < p1.y)
        return Direction::NORTH;
    if (p2.x == p1.x && p2.y > p1.y)
        return Direction::SOUTH;
    if (p2.x < p1.x && p2.y == p1.y)
        return Direction::WEST;
    if (p2.x > p1.x && p2.y == p1.y)
        return Direction::EAST;
    if (p2.x > p1.x && p2.y > p1.y)
        return Direction::SE;
    if (p2.x < p1.x && p2.y > p1.y)
        return Direction::SW;
    if (p2.x < p1.x && p2.y < p1.y)
        return Direction::NE;
    if (p2.x > p1.x && p2.y < p1.y)
        return Direction::NW;

    return Direction::LAST_DIRECTION;
}
