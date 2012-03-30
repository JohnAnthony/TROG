#ifndef ENTITY_H
#define ENTITY_H

#include "geometry.hpp"

class Entity {
    public:
    Point pos;
    int sight_range;
    int maxHP, curHP;
    int maxMP, curMP;
    int maxSTR, curSTR;
    int maxTOU, curTOU;
    int maxATT, curATT;
    int maxDEF, curDEF;
};

#endif
