#ifndef ENTITY_H
#define ENTITY_H

#include "geometry.hpp"

class Entity {
    public:
    bool isAlive(void);

    Point pos;
    char symbol;
    Colour colour;
    int baseHP;
    int baseMP;
    int curHP;
    int curMP;
    int baseSTR;
    int baseTOU;
    int baseATT;
    int baseDEF;
    int baseMAG;
    int baseWIL;
    int baseSIGHT;
    int baseMV;
};

#endif
