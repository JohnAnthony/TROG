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
      unsigned int inXP, const char *inName, const char *inDescript);
    std::string Description(void);
    void Attack(Character *c);
    void TakeDamage(int amt);
    bool isAlive(void);

    unsigned int Level;
    unsigned int XP_value;
    int mv_energy;
    const char *name;
    const char *descriptor;
    bool isActive;
    int maxHP;
    int curHP;
    int MP;
    int STR;
    int TOU;
    int ATT;
    int DEF;
    int MAG;
    int WIL;

};

static Enemy EnemyList[] = {

//lvl       sym     col         sight   hp  mp  str tou att def mag wil mv      xp      name
Enemy(1,    'o',    COL_BLUE,   6,      30, 0,  20, 20, 30, 30, 0,  30, 1000,   55,     "Binary Blob", "petty"),
Enemy(1,    'b',    COL_GREEN,  6,      30, 0,  20, 20, 30, 30, 0,  30, 800,    65,     "Bug", "Shallow"),
Enemy(2,    'd',    COL_RED,    6,      40, 0,  40, 40, 40, 40, 0,  40, 900,    185,    "Pigdog", "North American"),
Enemy(3,    'b',    COL_YELLOW, 6,      50, 0,  50, 50, 50, 50, 0,  50, 1200,   350,    "Neckbear", "juvenile"),
Enemy(4,    'G',    COL_YELLOW, 6,      60, 0,  60, 60, 60, 60, 0,  60, 1100,   450,    "Gnu", "wild"),
Enemy(5,    'D',    COL_YELLOW, 6,      70, 0,  70, 70, 70, 70, 0,  70, 1100,   700,    "Drorpion", "cursed"),
Enemy(6,    'O',    COL_YELLOW, 6,      80, 0,  80, 80, 80, 80, 0,  70, 1200,   850,    "Eaglebear", "screeching"),
Enemy(7,    'g',    COL_YELLOW, 6,      90, 0,  90, 90, 90, 90, 0,  90, 1000,   900,    "Doppelganger", "somebody else's"),
Enemy(8,    'p',    COL_RED,    6,      100,0,  100,100,100,100,0,  100,1000,   1050,   "Pegasus Pony", "little pink"),
Enemy(9,    'z',    COL_RED,    6,      110,0,  110,110,110,110,0,  110,1200,   1200,   "Jesus", "zombified"),
Enemy(10,   'T',    COL_GREEN,  6,      200,0,  120,120,120,120,0,  120,1000,   1000,   "Troll", "eax"),

};

#endif
