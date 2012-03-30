#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.hpp"
#include "geometry.hpp"

class Character : public Entity {
    public:
    void MoveTo(Point p);
    int XP;
};

#endif
