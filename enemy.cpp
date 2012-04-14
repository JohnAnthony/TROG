#include "geometry.hpp"
#include "gui.hpp"
#include <sstream>
#include <ncurses.h>
#include <cstdlib>

Enemy::Enemy(void) {}

Enemy::Enemy(unsigned int inLVL, char inSymbol, Color inCol, int inSight,
  int hp, int mp, int str, int tou, int att, int def, int mag, int wil,
  int inMV, unsigned int inXP, const char *inName, const char *inDescript) {
    this->symbol = inSymbol;
    this->colour = inCol;
    this->sight_range = inSight;
    this->maxHP = this->curHP = hp;
    this->MP = mp;
    this->STR = str;
    this->TOU = tou;
    this->ATT = att;
    this->DEF = def;
    this->MAG = mag;
    this->WIL = wil;
    this->mv_cost = inMV;
    this->XP_value = inXP;
    this->name = inName;
    this->descriptor = inDescript;
    this->isActive = false;
    this->mv_energy = 0;
    this->Level = inLVL;
}

std::string
Enemy::Description(void) {
    std::stringstream ss;
    char c;

    c = this->descriptor[0];
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        ss << "an ";
    else
        ss << "a ";
    ss << this->descriptor << " " << this->name;
    return ss.str();
}

void
Enemy::Attack(Character *c) {
    GUI gui;
    std::stringstream ss;
    int dmg;

    ss << "The " << this->descriptor << " " << this->name << " attacks ";
    ss << c->name;

    // The attack
    dmg = rand() % this->ATT - rand() % c->curDEF;
    if (dmg <= 0)
        ss << " but misses";
    else
        dmg += rand() % (int)(this->STR) - rand() % c->curTOU;

    //The damage
    if (dmg <= 0)
        ss << " but it does no damage";
    else {
        ss << " and does " << dmg << " damage";
        c->curHP -= dmg;
    }

    gui.AddMessage(ss.str());
}

void
Enemy::TakeDamage(int amt) {
    this->curHP -= amt;
    this->isActive = true;
}

bool
Enemy::isAlive(void) {
    return curHP > 0;
}
