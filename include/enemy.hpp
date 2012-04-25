#ifndef ENEMY_H
#define ENEMY_H

#include "enemy_type.hpp"
#include "character.hpp"
#include <string>

/*
=================================================
TROG
Enemy creatures definitions
=================================================
*/
class Character;

class Enemy
{
    public:
    Enemy(void);
    Enemy(EnemyType *et);
    std::string Description(void);
    void Attack(Character *c);
    void TakeDamage(int amt);
    bool isAlive(void);
    char getSymbol(void);
    Colour getColour(void);

    EnemyType *parent_type;
    int curHP;
    int curMP;
    bool isActive;
    Point pos;
    int mv_energy;
};

#endif
