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

//lvl       sym     col         sight   hp  mp  str tou att def mag wil mv      xp      name
EnemyType(1,    'z',    COL_GREEN,  6,      30, 0,  20, 20, 30, 30, 0,  30, 1500,   55,     "Decrepit Zombie"),
EnemyType(1,    'r',    COL_YELLOW, 6,      30, 0,  20, 20, 30, 30, 0,  30, 1100,   65,     "Giant Rat"),
EnemyType(1,    'a',    COL_RED,    6,      30, 0,  20, 20, 30, 30, 0,  30, 1100,   65,     "Giant Fire Ant"),
EnemyType(2,    'b',    COL_BLUE,   20,     40, 0,  40, 40, 40, 40, 0,  40, 1100,   185,    "Giant Cave Bat"),
EnemyType(2,    's',    COL_WHITE,  6,      40, 0,  40, 40, 40, 40, 0,  40, 800,    225,    "Decrepit Skeleton"),
EnemyType(3,    'c',    COL_GREEN,  6,      50, 0,  50, 50, 50, 50, 0,  50, 1200,   350,    "Cave Crocodile"),
EnemyType(4,    'S',    COL_GREEN,  6,      60, 0,  60, 60, 60, 60, 0,  60, 1100,   450,    "Tomb Serpent"),
EnemyType(5,    's',    COL_PURPLE, 6,      70, 0,  70, 70, 70, 70, 0,  70, 1000,   700,    "Shade"),
EnemyType(6,    'O',    COL_YELLOW, 6,      80, 0,  80, 80, 80, 80, 0,  70, 1200,   850,    "Owlbear"),
EnemyType(7,    'g',    COL_PURPLE, 6,      90, 0,  90, 90, 90, 90, 0,  90, 1000,   900,    "Doppelganger"),
EnemyType(8,    'n',    COL_RED,    6,      100,0,  100,100,100,100,0,  100,1000,   1050,   "Fire Newt"),
EnemyType(9,    'R',    COL_WHITE,  6,      110,0,  110,110,110,110,0,  110,1200,   1200,   "Grave Robber"),
EnemyType(10,   'T',    COL_GREEN,  6,      200,0,  120,120,120,120,0,  120,1000,   1000,   "Cave Troll")

};

#endif
