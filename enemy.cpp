#include "enemy.hpp"

Enemy::Enemy(void) {}

Enemy::Enemy(int inLVL, char inSymbol, Color inCol, int inSight, int hp, int mp, int str,
  int tou, int att, int def, int mag, int wil, int inMV, int inXP, const char *inName) {
    this->symbol = inSymbol;
    this->colour = inCol;
    this->sight_range = inSight;
    this->maxHP = this->curHP = hp;
    this->maxMP = this->curMP = mp;
    this->maxSTR = this->curSTR = str;
    this->maxTOU = this->curTOU = tou;
    this->maxATT = this->curATT = att;
    this->maxDEF = this->curDEF = def;
    this->maxMAG = this->curMAG = mag;
    this->maxWIL = this->curWIL = wil;
    this->mv_cost = inMV;
    this->XP_value = inXP;
    this->name = inName;
    this->isActive = false;
}
