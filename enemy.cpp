#include "geometry.hpp"
#include "gui.hpp"
#include <sstream>
#include <ncurses.h>
#include <cstdlib>

Enemy::Enemy(void) {}

Enemy::Enemy(EnemyType *et) {
    this->parent_type = et;
    this->curHP = this->parent_type->baseHP;
    this->curMP = this->parent_type->baseMP;
    this->isActive = false;
    this->mv_energy = 0;
}

std::string
Enemy::Description(void) {
    std::stringstream ss;
    char c;

    c = this->parent_type->name[0];
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        ss << "an ";
    else
        ss << "a ";
    ss << this->parent_type->name;
    return ss.str();
}

void
Enemy::Attack(Character *c) {
    GUI gui;
    std::stringstream ss;
    int dmg;

    ss << "The " << this->parent_type->name << " attacks ";
    ss << c->name;

    // The attack
    dmg = rand() % this->parent_type->baseATT - rand() % c->curDEF;
    if (dmg <= 0)
        ss << " but misses";
    else {
        dmg += rand() % (int)(this->parent_type->baseSTR) - rand() % c->curTOU;

        //The damage
        if (dmg <= 0)
            ss << " but it does no damage";
        else {
            ss << " and does " << dmg << " damage";
            c->TakeDamage(dmg);
        }
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

char
Enemy::getSymbol(void) {
    return this->parent_type->symbol;
}

Colour
Enemy::getColour(void) {
    return this->parent_type->colour;
}
