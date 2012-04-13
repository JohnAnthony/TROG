#ifndef ENTITY_H
#define ENTITY_H

#include "geometry.hpp"

class Entity {
    public:
    bool isAlive(void);

    Point pos;
    char symbol;
    Color colour;
    int sight_range;
    int mv_cost;
};

#endif
