#ifndef ENEMY_H
#define ENEMY_H

#include "entity.hpp"
#include <string>

class Enemy : public Entity {
    public:
    Enemy(void);
    Enemy(int inLVL, char inSymbol, Color inCol, int inSight, int hp, int mp, int str,
      int tou, int att, int def, int mag, int wil, int inMV, int inXP, const char *inName);
    int Level;
    int XP_value;
    const char *name;
    bool isActive;
};

static Enemy EnemyList[] = {

//lvl       sym     col         sig     hp  mp  str tou att def mag wil mv      xp  name
Enemy(1,    'o',    COL_BLUE,   5,      1,  0,  5,  5,  10, 10, 0,  5,  950,    10, "Binary Blob"),
Enemy(1,    'B',    COL_YELLOW, 5,      10, 0,  10, 10, 5,  5,  0,  5,  1000,   15, "Neckbear"),


};
#endif
