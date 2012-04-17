#ifndef ENEMY_H
#define ENEMY_H

#include "entity.hpp"
#include "character.hpp"
#include <string>

class Character;

class Enemy : public Entity {
    public:
    Enemy(void);
    Enemy(unsigned int inLVL, char inSymbol, Color inCol, int inSight, int hp,
      int mp, int str, int tou, int att, int def, int mag, int wil, int inMV,
      unsigned int inXP, const char *inName);
    std::string Description(void);
    void Attack(Character *c);
    void TakeDamage(int amt);
    bool isAlive(void);

    unsigned int Level;
    unsigned int XP_value;
    int mv_energy;
    const char *name;
    bool isActive;
};

static Enemy EnemyList[] = {

//lvl       sym     col         sight   hp  mp  str tou att def mag wil mv      xp      name
Enemy(1,    'z',    COL_GREEN,  6,      30, 0,  20, 20, 30, 30, 0,  30, 1500,   55,     "Decrepit Zombie"),
Enemy(1,    'a',    COL_RED,    6,      30, 0,  20, 20, 30, 30, 0,  30, 1100,   65,     "Giant Fire Ant"),
Enemy(2,    'b',    COL_BLUE,   20,     40, 0,  40, 40, 40, 40, 0,  40, 1100,   185,    "Giant Cave Bat"),
Enemy(2,    's',    COL_WHITE,  6,      40, 0,  40, 40, 40, 40, 0,  40, 800,    225,    "Decrepit Skeleton"),
Enemy(3,    'c',    COL_GREEN,  6,      50, 0,  50, 50, 50, 50, 0,  50, 1200,   350,    "Cave Crocodile"),
Enemy(4,    'G',    COL_YELLOW, 6,      60, 0,  60, 60, 60, 60, 0,  60, 1100,   450,    "Gnu"),
Enemy(5,    'D',    COL_YELLOW, 6,      70, 0,  70, 70, 70, 70, 0,  70, 1100,   700,    "Drorpion"),
Enemy(6,    'O',    COL_YELLOW, 6,      80, 0,  80, 80, 80, 80, 0,  70, 1200,   850,    "Eaglebear"),
Enemy(7,    'g',    COL_YELLOW, 6,      90, 0,  90, 90, 90, 90, 0,  90, 1000,   900,    "Doppelganger"),
Enemy(8,    'p',    COL_RED,    6,      100,0,  100,100,100,100,0,  100,1000,   1050,   "Pegasus Pony"),
Enemy(9,    'z',    COL_RED,    6,      110,0,  110,110,110,110,0,  110,1200,   1200,   "Jesus"),
Enemy(10,   'T',    COL_GREEN,  6,      200,0,  120,120,120,120,0,  120,1000,   1000,   "Troll")

};

#endif
