#include "enemy.hpp"
#include "geometry.hpp"
#include "gui.hpp"
#include <sstream>
#include <ncurses.h>

Enemy::Enemy(void) {}

Enemy::Enemy(unsigned int inLVL, char inSymbol, Color inCol, int inSight,
  int hp, int mp, int str, int tou, int att, int def, int mag, int wil,
  int inMV, unsigned int inXP, const char *inName, const char *inDescript) {
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
    this->descriptor = inDescript;
    this->isActive = false;
    this->mv_energy = 0;
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

    ss << "The " << this->descriptor << " " << this->name << " attacks ";
    ss << c->name;


    ss << " <continue>";
    gui.SetStatus(ss.str());
    getch();
}

void
Enemy::TakeDamage(int amt) {
    this->curHP -= amt;
    this->isActive = true;
}
