#include "equippable.hpp"
#include "gui.hpp"
#include "geometry.hpp"
#include <sstream>

static char const * const EquippableNames[Equippable::LAST_CATEGORY] = {
    "Dagger",
    "Short Sword",
    "Longsword",
    "Mace",
    "Flail",
    "Staff",
    "Two-Handed Sword",
    "Two-Handed Axe",
    "Light Shield",
    "Heavy Shield",
    "Robes",
    "Leather Armour",
    "Chainmail",
    "Platemail",
    "Holy Symbol"
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
    this->colour = COL_CYAN;
    this->type = Item::EQUIPPABLE;

    switch (inCat) {
        case Equippable::DAGGER:
            this->modSTR = (5 + DICEROLL(pot, 10)) / 4;
            this->modATT = 5 + DICEROLL(pot, 10);
            this->location = WEAPON;
            break;
        case Equippable::SHORT_SWORD:
            this->modSTR = (5 + DICEROLL(pot, 10)) / 2;
            this->modATT = 5 + DICEROLL(pot, 10);
            this->location = WEAPON;
            break;
        case Equippable::LONGSWORD:
            this->modSTR = 5 + DICEROLL(pot, 10);
            this->modATT = (5 + DICEROLL(pot, 10)) / 2;
            this->location = WEAPON;
            break;
        case Equippable::MACE:
        case Equippable::FLAIL:
            this->modSTR = (5 + DICEROLL(pot, 10)) / 2;
            this->modATT = (5 + DICEROLL(pot, 10)) / 2;
            this->location = WEAPON;
            break;
        case Equippable::STAFF:
            this->modSTR = (5 + DICEROLL(pot, 10)) / 4;
            this->modATT = (5 + DICEROLL(pot, 10)) / 4;
            this->modMAG = 5 + DICEROLL(pot, 10);
            this->location = WEAPON;
            break;
        case Equippable::TWO_HANDED_SWORD:
        case Equippable::TWO_HANDED_AXE:
            this->modSTR = (5 + DICEROLL(pot, 10)) * 1.5;
            this->modATT = 5 + DICEROLL(pot, 10);
            this->location = WEAPON;
            break;
        case Equippable::LIGHT_SHIELD:
            this->modDEF = 5 + DICEROLL(pot, 10);
            this->location = SHIELD;
            break;
        case Equippable::HEAVY_SHIELD:
            this->modDEF = 5 + DICEROLL(pot, 10) / 2;
            this->modTOU = 5 + DICEROLL(pot, 10) / 2;
            this->location = SHIELD;
            break;
        case Equippable::ROBES:
            this->modMAG = 5 + DICEROLL(pot, 10) / 2;
            this->modWIL = 5 + DICEROLL(pot, 10) / 2;
            this->location = BODY;
            break;
        case Equippable::LEATHER_ARMOUR:
            this->modTOU = 5 + DICEROLL(pot, 10) / 2;
            this->location = BODY;
            this->location = BODY;
            break;
        case Equippable::CHAINMAIL:
            this->modTOU = 5 + DICEROLL(pot, 10);
            this->location = BODY;
            break;
        case Equippable::PLATEMAIL:
            this->modTOU = 5 + DICEROLL(pot, 10) * 1.5;
            this->modDEF = 5 + DICEROLL(pot, 10) * (-0.5);
            this->location = BODY;
            break;
        case Equippable::HOLY_SYMBOL:
            this->modWIL = 5 + DICEROLL(pot, 10);
            this->location = NECK;
            break;
        case Equippable::LAST_CATEGORY:
            GUI::Alert("Error generating item!");
            break;
    }

    switch (this->location) {
        case HEAD:
            this->symbol = 'h';
            break;
        case BODY:
            this->symbol = 'a';
            break;
        case WEAPON:
            this->symbol = 'w';
            break;
        case SHIELD:
            this->symbol = 's';
            break;
        case GLOVES:
            this->symbol = 'g';
            break;
        case BOOTS:
            this->symbol = 'b';
            break;
        case RING1:
        case RING2:
            this->symbol = 'r';
            break;
        case NECK:
            this->symbol = 'n';
            break;
        case LAST_EQUIP_LOCATION:
        default:
            this->symbol = 'i';
            break;
    }

}

std::string 
Equippable::GetName(void) {
    std::stringstream ss;
    ss << EquippableNames[this->category];
    return ss.str();
}

std::string
Equippable::getNameWithQuality(void) {
    std::stringstream ss;
    ss << this->GetName();
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

Equippable*
Equippable::RandomEquippable(int pot) {
    return new Equippable((Equippable::Category)(rand() % 
      Equippable::LAST_CATEGORY), pot);
}
