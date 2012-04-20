#ifndef ENEMY_TYPE_H
#define ENEMY_TYPE_H

#include "geometry.hpp"

class EnemyType {
    public:
    EnemyType(unsigned int inLVL, char inSymbol, Colour inCol, int inSight, int hp,
      int mp, int str, int tou, int att, int def, int mag, int wil, int inMV,
      unsigned int inXP, const char *inName);

    unsigned int Level;
    unsigned int XP_value;
    const char *name;
    char symbol;
    Colour colour;
    int baseHP;
    int baseMP;
    int baseSTR;
    int baseTOU;
    int baseATT;
    int baseDEF;
    int baseMAG;
    int baseWIL;
    int baseMV;
    int baseSIGHT;
};

static EnemyType EnemyTypeList[] = {

        //lvl   sym     col         sight   hp  mp  str tou att def mag wil mv      xp      name
EnemyType(1,    'z',    COL_GREEN,  10,     5,  0,  10, 5,  10, 5,  0,  5,  1500,   55,     "Decrepit Zombie"),
EnemyType(1,    'r',    COL_YELLOW, 10,     5,  0,  10, 5,  10, 5,  0,  5,  1100,   65,     "Giant Rat"),
EnemyType(1,    'a',    COL_RED,    10,     5,  0,  10, 5,  10, 5,  0,  5,  1100,   65,     "Giant Fire Ant"),
EnemyType(2,    'b',    COL_BLUE,   20,     15, 0,  15, 7,  15, 7,  0,  7,  1100,   185,    "Giant Cave Bat"),
EnemyType(2,    's',    COL_WHITE,  10,     15, 0,  15, 7,  15, 7,  0,  7,  800,    225,    "Decrepit Skeleton"),
EnemyType(3,    'c',    COL_GREEN,  10,     25, 0,  20, 10, 20, 10, 0,  10, 1200,   350,    "Cave Crocodile"),
EnemyType(4,    'S',    COL_GREEN,  10,     40, 0,  25, 12, 25, 12, 0,  12, 1100,   450,    "Tomb Serpent"),
EnemyType(5,    's',    COL_PURPLE, 10,     60, 0,  30, 15, 30, 15, 0,  15, 1000,   700,    "Shade"),
EnemyType(6,    'O',    COL_YELLOW, 10,     85, 0,  35, 17, 35, 17, 0,  17, 1200,   850,    "Owlbear"),
EnemyType(7,    'g',    COL_PURPLE, 10,     105,0,  40, 20, 40, 20, 0,  20, 1000,   900,    "Doppelganger"),
EnemyType(8,    'n',    COL_RED,    10,     130,0,  45, 22, 45, 22, 0,  22, 1000,   1050,   "Fire Newt"),
EnemyType(9,    'R',    COL_WHITE,  10,     160,0,  50, 25, 50, 25, 0,  25, 1200,   1200,   "Grave Robber"),
EnemyType(10,   'T',    COL_GREEN,  10,     200,0,  55, 27, 55, 27, 0,  27, 1000,   1000,   "Cave Troll")

};

#endif
