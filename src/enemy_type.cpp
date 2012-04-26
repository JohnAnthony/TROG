#include "enemy_type.hpp"

EnemyType::EnemyType(unsigned int inLVL, char inSymbol, Colour inCol,
int inSight, int hp, int mp, int str, int tou, int att, int def, int mag,
int wil, int inMV, unsigned int inXP, EnemyType::Category inCat,
const char *inName) :
    Level(inLVL),
    XP_value(inXP),
    name(inName),
    symbol(inSymbol),
    colour(inCol),
    baseHP(hp),
    baseMP(mp),
    baseSTR(str),
    baseTOU(tou),
    baseATT(att),
    baseDEF(def),
    baseMAG(mag),
    baseWIL(wil),
    baseMV(inMV),
    baseSIGHT(inSight),
    category(inCat) {
}
