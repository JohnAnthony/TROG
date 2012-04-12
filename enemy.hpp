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

    unsigned int Level;
    unsigned int XP_value;
    int mv_energy;
    const char *name;
    const char *descriptor;
    bool isActive;
};

static Enemy EnemyList[] = {

//lvl       sym     col         sight   hp  mp  str tou att def mag wil mv      xp      name
Enemy(1,    'o',    COL_BLUE,   6,      1,  0,  5,  5,  10, 10, 0,  5,  950,    25,     "Binary Blob", "petty"),
Enemy(1,    'b',    COL_GREEN,  6,      8,  0,  8,  8,  8,  8,  0,  5,  1000,   85,     "Bug", "Shallow-Shell"),
Enemy(2,    'd',    COL_RED,    6,      14, 0,  12, 12, 12, 12, 0,  8,  900,    135,    "Pigdog", "North American"),
Enemy(3,    'b',    COL_YELLOW, 6,      20, 0,  20, 20, 16, 16, 0,  12, 1200,   350,    "Neckbear", "juvenile"),

};
#endif
