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
Enemy(1,    'o',    COL_BLUE,   5,      1,  0,  5,  5,  10, 10, 0,  5,  950,    25,     "Binary Blob", "petty"),
Enemy(1,    'd',    COL_RED,    5,      8,  0,  8,  8,  8,  8,  0,  5,  900,    50,     "Pigdog", "North American"),
Enemy(2,    'b',    COL_YELLOW, 5,      10, 0,  10, 10, 6,  6,  0,  5,  1200,   115,    "Neckbear", "juvenile"),

};
#endif
