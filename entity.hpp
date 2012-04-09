#ifndef ENTITY_H
#define ENTITY_H

#include "geometry.hpp"

class Entity {
    public:
    void TakeDamage(int amt);

    Point pos;
    char symbol;
    Color colour;
    int sight_range;
    int maxHP, curHP;
    int maxMP, curMP;
    int maxSTR, curSTR;
    int maxTOU, curTOU;
    int maxATT, curATT;
    int maxDEF, curDEF;
    int maxMAG, curMAG;
    int maxWIL, curWIL;
    int mv_cost;
};

#endif
