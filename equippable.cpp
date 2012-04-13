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
            this->modSTR = 20 + DICEROLL(pot, 10);
            this->modATT = 20 + DICEROLL(pot, 10);
            break;
        case Equippable::LIGHT_SHIELD:
            this->modDEF = 20 + DICEROLL(pot, 10);
            break;
        case Equippable::LIGHT_CHAIN:
            this->modTOU = 20 + DICEROLL(pot, 10);
            break;
        case Equippable::HOLY_SYMBOL:
            this->modWIL = 20 + DICEROLL(pot, 10);
            break;
        case Equippable::STAFF:
            this->modMAG = 20 + DICEROLL(pot, 10);
            break;
        case Equippable::LAST_CATEGORY:
            GUI::Alert("Error generating item!");
            break;
    }
}
