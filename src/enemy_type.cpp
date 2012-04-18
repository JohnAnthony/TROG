#include "enemy_type.hpp"

EnemyType::EnemyType(unsigned int inLVL, char inSymbol, Colour inCol,
  int inSight, int hp, int mp, int str, int tou, int att, int def, int mag,
  int wil, int inMV, unsigned int inXP, const char *inName) {
    this->symbol = inSymbol;
    this->colour = inCol;
    this->baseSIGHT = inSight;
    this->baseHP = hp;
    this->baseMP = mp;
    this->baseSTR = str;
    this->baseTOU = tou;
    this->baseATT = att;
    this->baseDEF = def;
    this->baseMAG = mag;
    this->baseWIL = wil;
    this->baseMV = inMV;
    this->XP_value = inXP;
    this->name = inName;
    this->Level = inLVL;
}
