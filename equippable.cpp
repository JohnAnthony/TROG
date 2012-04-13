#include "equippable.hpp"
#include "gui.hpp"
#include "geometry.hpp"

Equippable::Equippable(Equippable::Category inCat, int pot) {
    this->potency = pot;
    this->modHP = 0;
    this->modMP = 0;
    this->modSTR = 0;
    this->modTOU = 0;
    this->modATT = 0;
    this->modDEF = 0;
    this->modMAG = 0;
    this->modWIL = 0;

    switch (inCat) {
        case Equippable::SWORD:
            this->modSTR = 5 + DICEROLL(pot, 10);
            this->modATT = 5 + DICEROLL(pot, 10);
            this->location = WEAPON;
            break;
        case Equippable::LIGHT_SHIELD:
            this->modDEF = 5 + DICEROLL(pot, 10);
            this->location = SHIELD;
            break;
        case Equippable::LIGHT_CHAIN:
            this->modTOU = 5 + DICEROLL(pot, 10);
            this->location = BODY;
            break;
        case Equippable::HOLY_SYMBOL:
            this->modWIL = 5 + DICEROLL(pot, 10);
            this->location = NECK;
            break;
        case Equippable::STAFF:
            this->modMAG = 5 + DICEROLL(pot, 10);
            this->location = WEAPON;
            break;
        case Equippable::LAST_CATEGORY:
            GUI::Alert("Error generating item!");
            break;
    }
}

std::string 
Equippable::getName(void) {
    return "Derp";
}

int
Equippable::getQuality(void) {
    int ret;

    ret = 0;
    ret += this->modHP;
    ret += this->modMP;
    ret += this->modSTR;
    ret += this->modTOU;
    ret += this->modATT;
    ret += this->modDEF;
    ret += this->modMAG;
    ret += this->modWIL;

    return ret;
}
