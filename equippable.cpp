#include "equippable.hpp"
#include "gui.hpp"
#include "geometry.hpp"
#include <sstream>

static char const * const EquippableNames[Equippable::LAST_CATEGORY] = {
    "Short Sword",
    "Light Shield",
    "Light Chain",
    "Holy Symbol",
    "Staff"
};

Equippable::Equippable(Equippable::Category inCat, int pot) {
    this->potency = pot;
    this->category = inCat;
    this->modHP = 0;
    this->modMP = 0;
    this->modSTR = 0;
    this->modTOU = 0;
    this->modATT = 0;
    this->modDEF = 0;
    this->modMAG = 0;
    this->modWIL = 0;

    switch (inCat) {
        case Equippable::SHORT_SWORD:
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
    std::stringstream ss;
    ss << EquippableNames[this->category];
    return ss.str();
}

std::string
Equippable::getNameWithQuality(void) {
    std::stringstream ss;
    ss << this->getName();
    ss << " (q:" << this->getQuality() << ")";
    return ss.str();
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
